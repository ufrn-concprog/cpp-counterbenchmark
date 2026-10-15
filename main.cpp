#include <array>
using std::array;

#include <cstdio>
using std::printf;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "benchmark.h"
#include "counters.h"
#include "stats.h"

constexpr counter_t TOTAL_INCREMENTS = 5'000'000;  // divided among the threads
constexpr int MEASURED_RUNS = 20;
constexpr array<int, 5> THREAD_COUNTS{1, 2, 4, 8, 16};

/** Measures one mechanism at every thread count and prints mean, standard
 * deviation and correctness. */
template <typename Counter>
void run_benchmark(const string& label) {
    printf("=== %s ===\n", label.c_str());
    for (int threads : THREAD_COUNTS) {
        counter_t per_thread = TOTAL_INCREMENTS / threads;

        vector<double> times;
        int correct_runs = 0;
        for (int i = 0; i < MEASURED_RUNS; ++i) {
            Trial trial = run_trial<Counter>(threads, per_thread);
            times.push_back(trial.seconds);
            if (trial.correct) {
                ++correct_runs;
            }
        }

        double m = mean(times);
        printf("%2d threads | Mean: %.4f s | Standard Deviation: %.4f s | Correct: %d/%d\n",
            threads, m, std_dev(times, m), correct_runs, MEASURED_RUNS);
    }
    printf("\n");
}

int main() {
    run_benchmark<UnsafeCounter>("Unsynchronized");
    run_benchmark<MutexCounter>("Mutex");
    run_benchmark<AtomicCounter>("Atomic");
    run_benchmark<SpinLockCounter>("Spin lock");
    return 0;
}