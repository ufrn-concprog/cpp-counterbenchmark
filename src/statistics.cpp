/**
 * @file statistics.cpp
 * @brief Implements summary statistics for benchmark measurements.
 */

#include "counterbenchmark/statistics.h"

#include <cmath>

namespace counterbenchmark {

double mean(const std::vector<double>& values) {
    double sum = 0;
    for (double value : values) {
        sum += value;
    }
    return sum / values.size();
}

double std_dev(const std::vector<double>& values, double average) {
    double sum_squared_diffs = 0;
    for (double value : values) {
        const double difference = value - average;
        sum_squared_diffs += difference * difference;
    }
    return std::sqrt(sum_squared_diffs / (values.size() - 1));
}

}  // namespace counterbenchmark
