#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <barrier>
using std::barrier;

#include <chrono>
using std::chrono::steady_clock;
using std::chrono::duration;

#include <thread>
    using std::thread;

#include <vector>
using std::vector;

#include "counters.h"
#include "stats.h"

/** Outcome of one trial. */
struct Trial {
    double seconds;  // from the moment all threads were released until all had finished
    bool correct;    // whether the final count equals the number of increments performed
};

/**
 * Runs one timed trial of several threads incrementing a shared counter of type
 * Counter.
 *
 * All threads wait at a barrier together with the calling thread, so they begin 
 * incrementing at the same time and contention is not hidden by staggered thread start-up.
 * Timing starts when the gate opens.
 */
template <typename Counter>
Trial run_trial(int num_threads, counter_t increments_per_thread) {
    Counter counter;
    barrier start_line(num_threads + 1);  // workers + this thread

    vector<thread> workers;
    for (int t = 0; t < num_threads; ++t) {
        workers.emplace_back([&] {
            start_line.arrive_and_wait();
            for (counter_t i = 0; i < increments_per_thread; ++i) {
                counter.increment();
            }
        });
    }

    start_line.arrive_and_wait();  // opens the gate
    auto start = steady_clock::now();
    for (auto& worker : workers) {
        worker.join();
    }
    auto end = steady_clock::now();

    return Trial{duration<double>(end - start).count(),
                 counter.get() == num_threads * increments_per_thread};
}

#endif