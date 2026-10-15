#ifndef STATS_H
#define STATS_H

#include <cmath>
using std::sqrt;

#include <vector>
using std::vector;

/** Computes the mean of a vector of values. */
inline double mean(const vector<double>& values) {
    double sum = 0;
    for (double v : values) {
        sum += v;
    }
    return sum / values.size();
}

/** Computes the sample standard deviation of a vector of values. */
inline double std_dev(const vector<double>& values, double m) {
    double sum_squared_diffs = 0;
    for (double v : values) {
        sum_squared_diffs += (v - m) * (v - m);
    }
    return sqrt(sum_squared_diffs / (values.size() - 1));
}

#endif