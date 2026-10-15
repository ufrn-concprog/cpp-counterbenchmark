/**
 * @file    benchmark.cpp
 * @brief   Implements the synchronized multithreaded counter trial runner.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#include "benchmark.h"

#include <barrier>
using std::barrier;

#include <chrono>
using std::chrono::steady_clock;
using std::chrono::duration;

#include <thread>
using std::thread;

#include <vector>
using std::vector;

#include "atomic_counter.h"
#include "mutex_counter.h"
#include "spinlock_counter.h"
#include "unsafe_counter.h"

template <typename Counter>
Trial run_trial(int num_threads, counter_t increments_per_thread) {
    Counter counter;
    barrier start_line(num_threads + 1);

    vector<thread> workers;
    for (int t = 0; t < num_threads; ++t) {
        workers.emplace_back([&] {
            start_line.arrive_and_wait();
            for (counter_t i = 0; i < increments_per_thread; ++i) {
                counter.increment();
            }
        });
    }

    start_line.arrive_and_wait();
    const auto start = steady_clock::now();
    for (auto& worker : workers) {
        worker.join();
    }
    const auto end = steady_clock::now();

    return Trial{duration<double>(end - start).count(),
                 counter.get() == num_threads * increments_per_thread};
}

// Explicit instantiations for every supported counter type.
/// @cond
template Trial run_trial<UnsafeCounter>(int, counter_t);
template Trial run_trial<MutexCounter>(int, counter_t);
template Trial run_trial<AtomicCounter>(int, counter_t);
template Trial run_trial<SpinLockCounter>(int, counter_t);
/// @endcond
