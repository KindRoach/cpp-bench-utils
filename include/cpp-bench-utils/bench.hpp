#pragma once

#include <chrono>
#include <functional>
#include <iostream>

namespace cbu
{
    void print_human_readable_time_usage(size_t numIterations, std::chrono::microseconds totalDuration)
    {
        double avgDurationMicroSec = totalDuration.count() / static_cast<double>(numIterations);
        if (avgDurationMicroSec < 1000)
        {
            std::cout << numIterations << " iters @ avg: " << avgDurationMicroSec << " us" << "\n";
        }
        else if (avgDurationMicroSec < 1000000)
        {
            std::cout << numIterations << " iters @ avg: " << avgDurationMicroSec / 1000.0 << " ms" << "\n";
        }
        else
        {
            std::cout << numIterations << " iters @ avg: " << avgDurationMicroSec / 1000000.0 << " s" << "\n";
        }
    }

    void benchmark_func(
        size_t num_iter,
        const std::function<void()>& func,
        double warmup_ratio = 0.1
    )
    {
        if (num_iter <= 1)
        {
            std::cerr << "Warning: num_iter less than 2, running func once.\n";
            func();
            return;
        }

        // Warm-up phase
        int warm_up_iter = std::max(1, static_cast<int>(num_iter * warmup_ratio));
        for (int i = 0; i < warm_up_iter; ++i)
        {
            func();
        }

        // benchmark phase
        num_iter -= warm_up_iter;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_iter; ++i)
        {
            func();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        print_human_readable_time_usage(num_iter, totalDuration);
    }

    struct BenchmarkOptions
    {
        double warmup_ratio = 0.1;
        size_t total_mem_bytes = 0;
        size_t total_flop = 0;
    };

    void benchmark_func_by_time(
        size_t total_seconds,
        const std::function<void()>& func,
        const BenchmarkOptions& opt = {}
    )
    {
        if (total_seconds <= 0)
        {
            std::cerr << "Warning: total_seconds not lagger than 0, running func once.\n";
            func();
            return;
        }

        using clock = std::chrono::high_resolution_clock;

        // Warm-up phase
        double warmup_seconds = total_seconds * opt.warmup_ratio;
        auto warmup_end = clock::now() + std::chrono::duration<double>(warmup_seconds);
        while (clock::now() < warmup_end)
        {
            func();
        }

        // Benchmark phase
        double benchmark_seconds = total_seconds * (1.0 - opt.warmup_ratio);
        auto bench_end = clock::now() + std::chrono::duration<double>(benchmark_seconds);

        int num_iter = 0;
        auto start = clock::now();
        while (clock::now() < bench_end)
        {
            func();
            ++num_iter;
        }
        auto end = clock::now();

        auto totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        print_human_readable_time_usage(num_iter, totalDuration);

        if (num_iter > 0 && opt.total_mem_bytes > 0.0)
        {
            double avg_time_s = totalDuration.count() / 1e6 / num_iter;
            double mem_bw = opt.total_mem_bytes / avg_time_s / 1e9;
            std::cout << "Memory Bandwidth: " << mem_bw << " GB/s" << "\n";
        }
        if (num_iter > 0 && opt.total_flop > 0.0)
        {
            double avg_time_s = totalDuration.count() / 1e6 / num_iter;
            double compute_perf = opt.total_flop / avg_time_s / 1e12;
            std::cout << "Compute Perf: " << compute_perf << " TFLOP/s" << "\n";
        }
    }
}
