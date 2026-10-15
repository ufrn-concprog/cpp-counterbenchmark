/**
 * @file    statistics.cpp
 * @brief   Implements summary statistics for benchmark measurements.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#include "statistics.h"

#include <cmath>
using std::sqrt;

/**
 * @brief Computes the arithmetic mean of a collection of values.
 * @param values Measurement values; must not be empty.
 * @return The sum of the values divided by their count.
 */
double mean(const vector<double>& values) {
    double sum = 0;
    for (double value : values) {
        sum += value;
    }
    return sum / values.size();
}

/**
 * @brief Computes the sample standard deviation of a collection of values.
 * @param values Measurement values; must contain at least two elements.
 * @param average Arithmetic mean of @p values.
 * @return The square root of the sum of squared deviations divided by
 *         `values.size() - 1`.
 */
double std_dev(const vector<double>& values, double average) {
    double sum_squared_diffs = 0;
    for (double value : values) {
        const double difference = value - average;
        sum_squared_diffs += difference * difference;
    }
    return sqrt(sum_squared_diffs / (values.size() - 1));
}
