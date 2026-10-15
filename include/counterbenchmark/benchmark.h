/**
 * @file benchmark.h
 * @brief Provides a synchronized multithreaded counter trial runner.
 */

#ifndef COUNTERBENCHMARK_BENCHMARK_H
#define COUNTERBENCHMARK_BENCHMARK_H

#include <barrier>
#include <chrono>
#include <thread>
#include <vector>

#include "counterbenchmark/counter.h"

namespace counterbenchmark {

/**
 * Result of one benchmark trial.
 */
struct Trial {
    /// Elapsed wall-clock time in seconds.
    double seconds;
    /// Whether the final counter value matched the expected increment count.
    bool correct;
};

/**
 * Runs one timed trial in which worker threads increment a shared counter.
 *
 * All workers synchronize with the calling thread at a barrier before
 * incrementing, avoiding staggered worker startup. The elapsed time covers
 * the interval from the caller's barrier release through completion of all
 * worker joins.
 *
 * @tparam Counter Counter type providing `increment()` and `get()` operations.
 * @param num_threads Number of worker threads; must be greater than zero.
 * @param increments_per_thread Number of increments each worker performs;
 *        must be non-negative.
 * @return The elapsed time and whether the final value matched
 *         `num_threads * increments_per_thread`.
 */
template <typename Counter>
Trial run_trial(int num_threads, counter_t increments_per_thread) {
    Counter counter;
    std::barrier start_line(num_threads + 1);

    std::vector<std::thread> workers;
    for (int t = 0; t < num_threads; ++t) {
        workers.emplace_back([&] {
            start_line.arrive_and_wait();
            for (counter_t i = 0; i < increments_per_thread; ++i) {
                counter.increment();
            }
        });
    }

    start_line.arrive_and_wait();
    const auto start = std::chrono::steady_clock::now();
    for (auto& worker : workers) {
        worker.join();
    }
    const auto end = std::chrono::steady_clock::now();

    return Trial{std::chrono::duration<double>(end - start).count(),
                 counter.get() == num_threads * increments_per_thread};
}

}  // namespace counterbenchmark

#endif
