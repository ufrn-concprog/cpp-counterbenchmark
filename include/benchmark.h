/**
 * @file benchmark.h
 * @brief Provides a synchronized multithreaded counter trial runner.
 */

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "counter.h"

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
 * Explicitly instantiated in benchmark.cpp for UnsafeCounter, MutexCounter,
 * AtomicCounter, and SpinLockCounter. Supporting another counter type requires
 * adding an instantiation there.
 *
 * @tparam Counter Counter type providing `increment()` and `get()` operations.
 * @param num_threads Number of worker threads; must be greater than zero.
 * @param increments_per_thread Number of increments each worker performs;
 *        must be non-negative.
 * @return The elapsed time and whether the final value matched
 *         `num_threads * increments_per_thread`.
 */
template <typename Counter>
Trial run_trial(int num_threads, counter_t increments_per_thread);

#endif
