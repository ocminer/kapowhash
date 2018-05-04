// Ethash: C/C++ implementation of Ethash, the Ethereum Proof of Work algorithm.
// Copyright 2018 Pawel Bylica.
// Licensed under the Apache License, Version 2.0. See the LICENSE file.

#include "keccak_utils.hpp"

#include <benchmark/benchmark.h>


void fake_keccakf1600(uint64_t* state) noexcept
{
    // Do nothing to measure performance of the code outside of keccakf function.
    (void)state;
}

template<void keccak_fn(uint64_t*, const uint8_t*, size_t)>
static void fake_keccak256(benchmark::State& state)
{
    std::vector<uint8_t> data(static_cast<size_t>(state.range(0)), 0xaa);

    for (auto _ : state)
    {
        uint64_t out[4];
        keccak_fn(out, data.data(), data.size());
        benchmark::DoNotOptimize(out);
    }
}
BENCHMARK_TEMPLATE(fake_keccak256, fake_keccak256_default_aligned)->Arg(128)->Arg(17 * 8)->Arg(4096)->Arg(16 * 1024);
BENCHMARK_TEMPLATE(fake_keccak256, fake_keccak256_default)->Arg(128)->Arg(17 * 8)->Arg(4096)->Arg(16 * 1024);
BENCHMARK_TEMPLATE(fake_keccak256, fake_keccak256_fastest)->Arg(128)->Arg(17 * 8)->Arg(4096)->Arg(16 * 1024);


template<void keccak_fn(uint64_t*, const uint8_t*, size_t)>
static void fake_keccak256_unaligned(benchmark::State& state)
{
    const auto size = static_cast<size_t>(state.range(0));
    std::vector<uint8_t> data(size + 1, 0xaa);

    for (auto _ : state)
    {
        uint64_t out[4];
        keccak_fn(out, data.data() + 1, size);
        benchmark::DoNotOptimize(out);
    }
}
BENCHMARK_TEMPLATE(fake_keccak256_unaligned, fake_keccak256_default)->Arg(128)->Arg(17 * 8)->Arg(4096)->Arg(16 * 1024);
BENCHMARK_TEMPLATE(fake_keccak256_unaligned, fake_keccak256_fastest)->Arg(128)->Arg(17 * 8)->Arg(4096)->Arg(16 * 1024);