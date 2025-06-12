#pragma once

static size_t flops_total = 0;

inline void increment_flops() { flops_total++; }
inline void increment_n_flops(size_t n) { flops_total += n; }

inline size_t get_n_flops() { return flops_total; }