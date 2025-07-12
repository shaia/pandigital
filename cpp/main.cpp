/**
 * @file main.cpp
 * @brief Main program entry point and benchmarking framework for pandigital number calculator
 * 
 * This file contains:
 * 1. CPU feature detection for SIMD instructions
 * 2. Benchmarking framework for comparing implementations
 * 3. Result presentation and formatting
 * 
 * The program compares different implementations:
 * - Simple (sequential)
 * - Base SIMD (AVX2)
 * - Advanced AVX2
 * - AVX-512 (when supported)
 */

#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>
#include "calc_result.h"

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace impl {
    namespace simple { CalcResult calc(); }  // Add this line
    namespace base_simd { CalcResult calc(); }
    namespace avx2 { CalcResult calc(); }
    namespace avx512 { CalcResult calc(); }
    namespace avx2_advanced { CalcResult calc(); }
}

/**
 * @brief Checks CPU support for advanced SIMD features
 * @param checkAVX512 Whether to check for AVX-512 support
 * @return true if required features are supported
 */
bool check_cpu_features(bool checkAVX512 = true) {
#ifdef _MSC_VER
    int cpuInfo[4];
    __cpuid(cpuInfo, 0);
    if (cpuInfo[0] >= 7) {
        __cpuidex(cpuInfo, 7, 0);
        bool hasAVX2 = (cpuInfo[1] & (1 << 5)) != 0;
        if (checkAVX512) {
            bool hasAVX512F = (cpuInfo[1] & (1 << 16)) != 0;
            return hasAVX512F;
        }
        return hasAVX2;
    }
    return false;
#else
    return false;
#endif
}

/**
 * @brief Benchmarks a function with multiple iterations
 * @param func Function to benchmark
 * @param iterations Number of iterations to run
 * @return Tuple of (average time, result, consistency check)
 */
template<typename Func>
auto benchmark(Func&& func, int iterations = 5) {
    std::vector<double> times;
    std::vector<impl::CalcResult> results;
    
    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = func();
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> duration = end - start;
        times.push_back(duration.count());
        results.push_back(result);
    }
    
    double avgTime = std::accumulate(times.begin(), times.end(), 0.0) / iterations;
    bool allSame = std::adjacent_find(results.begin(), results.end(),
        [](const auto& a, const auto& b) { 
            return a.maxVal != b.maxVal || a.bestK != b.bestK || a.bestN != b.bestN; 
        }) == results.end();
    
    return std::make_tuple(avgTime, results[0], allSame);
}

void printTableHeader() {
    std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;
    std::cout << std::setfill(' ')
              << std::left << std::setw(20) << "Implementation"
              << std::right << std::setw(15) << "Max Value"
              << std::setw(10) << "Best K"
              << std::setw(10) << "Best N"
              << std::setw(15) << "Time (ms)"
              << std::setw(10) << "Valid" << std::endl;
    std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;
    std::cout << std::setfill(' ');
}

void printTableRow(const std::string& name, const impl::CalcResult& result, 
                  double time, bool consistent) {
    std::cout << std::left << std::setw(20) << name
              << std::right << std::setw(15) << result.maxVal
              << std::setw(10) << result.bestK
              << std::setw(10) << result.bestN
              << std::fixed << std::setprecision(3)
              << std::setw(15) << time
              << std::setw(10) << (consistent ? "Yes" : "No") << std::endl;
}

int main() {
    std::cout << "Program starting...\n" << std::endl;

    try {
        std::vector<std::pair<std::string, std::function<impl::CalcResult()>>> implementations;
        implementations.push_back({"Simple", impl::simple::calc});  // Add this line
        implementations.push_back({"Base SIMD", impl::base_simd::calc});
        implementations.push_back({"AVX2", impl::avx2::calc});

        bool hasAVX512 = check_cpu_features(true);
        if (hasAVX512) {
            implementations.push_back({"AVX-512", impl::avx512::calc});
        } else {
            std::cout << "CPU does not support AVX-512, skipping that implementation.\n" << std::endl;
        }

        std::cout << "Running implementations:\n" << std::endl;
        printTableHeader();

        for (const auto& [name, func] : implementations) {
            try {
                auto [avgTime, result, consistent] = benchmark(func);
                printTableRow(name, result, avgTime, consistent);
            } catch (const std::exception& e) {
                std::cout << name << " failed: " << e.what() << std::endl;
            } catch (...) {
                std::cout << name << " failed with unknown exception" << std::endl;
            }
        }

        std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "Main exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown main exception occurred" << std::endl;
    }

    return 0;
}