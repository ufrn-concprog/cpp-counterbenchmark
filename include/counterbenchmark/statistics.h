/**
 * @file statistics.h
 * @brief Declares summary-statistics functions for benchmark measurements.
 */

#ifndef COUNTERBENCHMARK_STATISTICS_H
#define COUNTERBENCHMARK_STATISTICS_H

#include <vector>

namespace counterbenchmark {

/**
 * Computes the arithmetic mean of a collection of values.
 *
 * @param values Measurement values; must not be empty.
 * @return The sum of the values divided by their count.
 */
double mean(const std::vector<double>& values);

/**
 * Computes the sample standard deviation of a collection of values.
 *
 * @param values Measurement values; must contain at least two elements.
 * @param average Arithmetic mean of @p values.
 * @return The square root of the sum of squared deviations divided by
 *         `values.size() - 1`.
 */
double std_dev(const std::vector<double>& values, double average);

}  // namespace counterbenchmark

#endif
