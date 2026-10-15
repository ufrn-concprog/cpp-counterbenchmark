/**
 * @file counter.h
 * @brief Defines the integer type used by counter implementations.
 */

#ifndef COUNTERBENCHMARK_COUNTER_H
#define COUNTERBENCHMARK_COUNTER_H

#include <cstdint>

namespace counterbenchmark {

/// Signed integer type used to store and report counter values.
using counter_t = std::int64_t;

}  // namespace counterbenchmark

#endif
