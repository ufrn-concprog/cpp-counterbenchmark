/**
 * @file    unsafe_counter.cpp
 * @brief   Implements the counter without mutual exclusion.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#include "unsafe_counter.h"

void UnsafeCounter::increment() {
    value.store(value.load(std::memory_order_relaxed) + 1,
                std::memory_order_relaxed);
}

counter_t UnsafeCounter::get() const {
    return value.load(std::memory_order_relaxed);
}
