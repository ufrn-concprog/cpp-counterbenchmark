/**
 * @file    main.cpp
 * @brief   Configures and runs the benchmark.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#include <array>
using std::array;

#include <cstdio>
using std::printf;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "atomic_counter.h"
#include "benchmark.h"
#include "mutex_counter.h"
#include "spinlock_counter.h"
#include "statistics.h"
#include "unsafe_counter.h"

namespace {

/** @brief Total requested increments per trial, divided among its worker threads. */
constexpr counter_t TOTAL_INCREMENTS = 5000000;

/** @brief Number of trials used to calculate each thread-count summary. */
constexpr int MEASURED_RUNS = 20;

/** @brief Worker-thread counts used for each counter implementation. */
constexpr std::array<int, 5> THREAD_COUNTS{1, 2, 4, 8, 16};

/**
 * Measures one counter implementation at each configured thread count.
 *
 * For every thread count, this function runs the configured number of trials
 * and prints the mean elapsed time, sample standard deviation, and number of
 * correct results.
 *
 * @tparam Counter Counter type accepted by run_trial.
 * @param label Display name for the counter implementation.
 */
template <typename Counter>
void run_benchmark(const string& label) {
    printf("=== %s ===\n", label.c_str());
    for (int threads : THREAD_COUNTS) {
        const counter_t per_thread = TOTAL_INCREMENTS / threads;
        vector<double> times;
        int correct_runs = 0;
        for (int i = 0; i < MEASURED_RUNS; ++i) {
            const Trial trial = run_trial<Counter>(threads, per_thread);
            times.push_back(trial.seconds);
            if (trial.correct) {
                ++correct_runs;
            }
        }

        const double average = mean(times);
        printf(
            "%2d threads | Mean: %.4f s | Standard Deviation: %.4f s | Correct: %d/%d\n",
            threads, average, std_dev(times, average),
            correct_runs, MEASURED_RUNS);
    }
    printf("\n");
}
}

/**
 * @brief   Runs the benchmark suite for all supported counter implementations.
 * @return  Exit status of the application; zero indicates successful completion.
 */
int main() {
    run_benchmark<UnsafeCounter>("Unsynchronized");
    run_benchmark<MutexCounter>("Mutex");
    run_benchmark<AtomicCounter>("Atomic");
    run_benchmark<SpinLockCounter>("Spin lock");
    return 0;
}
