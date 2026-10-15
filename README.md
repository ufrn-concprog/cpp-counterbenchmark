# Benchmarking Synchronization Mechanisms with a Shared Counter in C++

[![C++](https://img.shields.io/badge/C++-20%2B-green?logo=c%2B%2B)](https://en.cppreference.com/w/cpp/20)
![Build](https://img.shields.io/badge/build-manual-lightgrey)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

This benchmark measures the cost of four ways to increment a shared counter from several threads in C++: using no synchronization, a mutex, an atomic variable, and a spin lock. This repository serves a pedagogical purpose, showing that correctness and speed are different questions, and what contention costs.

This project is part of the **Concurrent Programming** module at the [Federal University of Rio Grande do Norte (UFRN)](https://www.ufrn.br), Natal, Brazil.

## 📃 About

### Mechanisms

All the mechanisms are implemented 

| Mechanism | Class | How it works | Correct? |
| :-------- | :----- | :---------- | :------- |
| Unsynchronized | [`UnsafeCounter`](include/unsafe_counter.h) | A separate relaxed atomic `load` and `store`, each safe on its own but not atomic together | No: it loses updates |
| Mutex | [`MutexCounter`](include/mutex_counter.h) | [`std::mutex`](https://en.cppreference.com/w/cpp/thread/mutex) guarded by a [`std::lock_guard`](https://en.cppreference.com/cpp/thread/lock_guard) | Yes |
| Atomic | [`AtomicCounter`](include/atomic_counter.h) | [`std::atomic<int64_t>::fetch_add`](https://en.cppreference.com/w/cpp/atomic/atomic/fetch_add), a hardware atomic read-modify-writ | Yes |
| Spin lock | [`SpinLockCounter`](include/spinlock_counter) | A [`std::atomic_flag`](https://en.cppreference.com/w/cpp/atomic/atomic_flag) with `test_and_set` (acquire) and `clear` (release) | Yes |

None of these classes share a common base class. The benchmark passes each class as a template argument, so the compiler resolves every call at compile time.
 
### Methodology
 
1. **Fixed total work.** Every configuration performs 5,000,000 increments in total, divided evenly among 1, 2, 4, 8, and 16 threads. The 1-thread case refers to each mechanism's *uncontended* cost, and the other rows show what contention adds.
2. **Starting gate.** The worker threads and the main thread wait at a [`std::barrier`](https://en.cppreference.com/w/cpp/thread/barrier), so all workers begin incrementing together. Timing starts when the gate opens and ends when all workers have finished. Thread creation is not timed.
3. **Warm-up.** For each configuration, 5 trials run and are discarded.
4. **Measurement.** 20 measured trials per configuration, reporting the mean and standard deviation.
5. **Correctness check.** After each trial, the final count is compared with the number of increments performed, and the number of correct trials is reported.

### Design Notes

* **Why the unsynchronized counter is not a plain `long`.** Concurrent unsynchronized access to a plain variable is a data race, which is undefined behavior in C++. At `-O2`, the compiler may also collapse the whole loop into a single addition, so the lost updates would not even appear. The relaxed `load` and `store` compile to ordinary memory accesses, so the cost is no synchronization, and the race remains observable.
* **Why a starting gate.** Staggered thread start-up would hide contention.
- **A spin lock is a lock.** If the thread holding it is descheduled, every other thread spins until the holder runs again. This is visible when threads outnumber cores. The *acquire* on `lock` and the *release* on `unlock` are what make one holder's writes visible to the next.
- **[`std::atomic_flag`](https://en.cppreference.com/w/cpp/atomic/atomic_flag)** is the one atomic type guaranteed to be lock-free.
- **No speedup is expected.** All threads increment the same counter, so adding threads cannot make the work faster. The benchmark measures the *cost of sharing*, not parallel speedup.

## 📂 Repository Structure
 
```
cpp-counterbenchmark/
└── doc/                          # HTML documentation
└── Doxyfile                      # Doxygen configuration file
└── include/
    ├── atomic_counter.h          # Declares a counter based on an atomic read-modify-write operation
    ├── benchmark.h               # Defines the trial runner
    ├── counter.h                 # Defines the integer type used by counter implementations
    ├── mutex_counter.h           # Declares a counter protected by a mutex
    ├── spinlock_counter.h        # Declares a counter protected by a spin lock
    ├── statistics.h              # Declares summary statistics functions for benchmark measurements
    ├── unsafe_counter.h          # Declares a counter without mutual exclusion
└── LICENSE
└── Makefile
└── main.cpp                      # Configures and runs the benchmark
└── README.md
└── src/
    ├── atomic_counter.cpp        # Implements the atomic read-modify-write counter
    ├── benchmark.cpp             # Implements the trial runner
    ├── mutex_counter.cpp         # Implements the mutex-protected counter
    ├── spinlock_counter.cpp      # Implements the spin-lock-protected counter
    ├── statistics.cpp            # Implements summary statistics for benchmark measurements
    ├── unsafe_counter.cpp        # Implements the counter without mutual exclusion
```
 
## 🚀 Getting Started
 
### Prerequisites
 
* A C++ compiler with C++20 support, such as GCC 11 or later, or a recent Clang
* A terminal or IDE

The program uses only the C++ standard library, so there are no additional dependencies.

### Compiling
 
From the project root:
 
```bash
make
```
 
### Running
 
```bash
./bin/counterbenchmark
```

All four mechanisms run in sequence.
 
The workload is configured by constants at the top of [`main.cpp`](main.cpp): 

| Constant            | Description | Value set |
| :-----------------  | :---------- | --------- |
| `TOTAL_INCREMENTS`  | Total increments per trial, divided among the threads (all thread counts divide it evenly) | 5,000,000 |
| `THREAD_COUNTS`     | Number of threads to use for each benchmark run | 1, 2, 4, 8, 16 |
| `MEASURED_RUNS`     | Number of runs to measure for each configuration | 20 |
 
### Output Example
 
```text
=== Atomic ===
 1 threads | Mean: 0.0087 s | Standard Deviation: 0.0001 s | Correct: 20/20
 2 threads | Mean: 0.0179 s | Standard Deviation: 0.0004 s | Correct: 20/20
 4 threads | Mean: 0.0336 s | Standard Deviation: 0.0002 s | Correct: 20/20
 8 threads | Mean: 0.1622 s | Standard Deviation: 0.0245 s | Correct: 20/20
16 threads | Mean: 0.1679 s | Standard Deviation: 0.0062 s | Correct: 20/20
```
 
The `Correct` column shows how many of the 20 measured trials ended with the right count.
 
These are hypotheses to check, not guaranteed results:
 
* **Unsynchronized:** correct with 1 thread and wrong with 2 or more threads; it is the fastest strategy due to zero synchronization overhead.
* **Mutex:** always correct; under contention, threads block and wake up.
* **Atomic:** always correct; it leverages lock-free hardware instructions for ultra-low execution times at low thread counts, but degrades as thread contention scales due to repeated compare-and-swap retry loops.
* **Spin lock:** it provides fast acquisition under minimal thread competition, but causes performance degradation at high thread counts as threads consume CPU time busy-waiting for lock release.

## 🧪 Things to Explore
 
1. Replace [`std::lock_guard`](https://en.cppreference.com/cpp/thread/lock_guard) with manual `lock()` and `unlock()` calls. What changes if the critical section can throw an exception?
2. Make the critical section longer (e.g., add some computation inside the lock). At what point does the spin lock stop being competitive?
 
## 🤝 Contributing
 
Contributions are welcome! Fork this repository and submit a pull request 🚀
 
## 📜 License
 
This project is licensed under the [MIT License](LICENSE).
