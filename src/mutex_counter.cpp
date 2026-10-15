/**
 * @file    mutex_counter.cpp
 * @brief   Implements the mutex-protected counter.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#include "mutex_counter.h"

void MutexCounter::increment() {
    std::lock_guard<std::mutex> guard(mtx);
    ++value;
}

counter_t MutexCounter::get() {
    std::lock_guard<std::mutex> guard(mtx);
    return value;
}
