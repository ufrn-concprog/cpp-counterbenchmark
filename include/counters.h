#ifndef COUNTERS_H
#define COUNTERS_H

#include <atomic>
using std::atomic;
using std::atomic_flag;
using std::memory_order_acquire;
using std::memory_order_relaxed;
using std::memory_order_release;

#include <cstdint>
using counter_t = std::int64_t;

#include <mutex>
using std::mutex;
using std::lock_guard;


/**
 * No mutual exclusion: loses updates when used by several threads.
 *
 * Implemented as a separate atomic load and atomic store, each individually
 * safe but not atomic together. A plain `long` would be a data race (undefined
 * behavior), and at -O2 the compiler may collapse the whole loop into a single
 * addition, hiding the lost updates. Relaxed operations compile to ordinary
 * loads and stores, so this has the cost of "no synchronization".
 */
class UnsafeCounter {
public:
    void increment() {
        value.store(value.load(memory_order_relaxed) + 1,
                    memory_order_relaxed);
    }
    counter_t get() const { return value.load(memory_order_relaxed); }

private:
    atomic<counter_t> value{0};
};

/** Protected by a mutex: a thread that finds it taken is suspended by the OS.
 */
class MutexCounter {
public:
    void increment() {
        lock_guard<mutex> guard(mtx);  // RAII: released at end of scope
        ++value;
    }
    counter_t get() {
        lock_guard<mutex> guard(mtx);
        return value;
    }

private:
    mutex mtx;
    counter_t value = 0;  // guarded by mtx
};

/** A single hardware atomic read-modify-write: no lock, and no thread ever
 * blocks. */
class AtomicCounter {
public:
    void increment() {
        // default memory order: sequentially consistent
        value.fetch_add(1);
    }
    counter_t get() const { return value.load(); }

private:
    atomic<counter_t> value{0};
};

/**
 * Protected by a spin lock: a thread that finds it taken keeps retrying instead
 * of sleeping. Acquire on lock and release on unlock are what make the writes
 * to `value` visible to the next holder. A spin lock is still a lock, not a
 * lock-free algorithm.
 */
class SpinLockCounter {
public:
    void increment() {
        lock();
        ++value;
        unlock();
    }
    counter_t get() {
        lock();
        counter_t result = value;
        unlock();
        return result;
    }

private:
    void lock() {
        while (flag.test_and_set(memory_order_acquire)) {
            // spin
        }
    }
    void unlock() { flag.clear(memory_order_release); }

    atomic_flag flag;
    counter_t value = 0;    // guarded by the spin lock
};

#endif