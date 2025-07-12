/**
 * @file pandigital_avx2.cpp
 * @brief Advanced AVX2 implementation with optimized digit processing
 * 
 * Optimizations:
 * 1. Bit manipulation for digit checking
 * 2. Pre-computed powers of 10
 * 3. Optimized digit counting
 * 4. SIMD parallel processing
 * 
 * Requirements:
 * - AVX2 instruction set
 * - FMA instructions (optional)
 * - 64-byte aligned memory
 * 
 * Performance characteristics:
 * - Processes 8 numbers per SIMD operation
 * - Minimized branching in critical paths
 * - Efficient bit manipulation for validation
 */
#include <immintrin.h>
#include <iostream>
#include <algorithm>
#include "calc_result.h"

namespace impl::avx2 {
    // Precomputed powers of ten for up to 9 digits
    static constexpr int POW10[10] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};

    // Count decimal digits of x (x > 0)
    inline int digitCount(int x) {
        int cnt = 0;
        do { ++cnt; x /= 10; } while (x);
        return cnt;
    }

    // Concatenate p1 and p2 into a single integer.
    inline int concatProducts(int p1, int p2) {
        int d2 = digitCount(p2);
        return p1 * POW10[d2] + p2;
    }

    // Check if p1 and p2 together form a 1–9 pandigital using bitmask
    inline bool isPandigitalMask(int p1, int p2) {
        int mask = 0;
        // mask digits of p1
        while (p1) {
            int d = p1 % 10;
            if (d == 0 || (mask & (1 << d))) return false;
            mask |= (1 << d);
            p1 /= 10;
        }
        // mask digits of p2
        while (p2) {
            int d = p2 % 10;
            if (mask & (1 << d)) return false;
            mask |= (1 << d);
            p2 /= 10;
        }
        // full mask for digits 1-9 is bits 1-9 set => 0x3FE
        return mask == 0x3FE;
    }

    // Process a batch of up to 8 k values starting at kStart
    void processBatch(int kStart, int batchSize, CalcResult& result) {
        alignas(64) int kArr[8];
        alignas(64) int prod1[8];
        alignas(64) int prod2[8];
        // Prepare k values
        for (int i = 0; i < batchSize; ++i) kArr[i] = kStart + i;
        for (int i = batchSize; i < 8; ++i) kArr[i] = 0;

        __m256i kVec = _mm256_loadu_si256(reinterpret_cast<__m256i *>(kArr));
        __m256i two  = _mm256_set1_epi32(2);
        __m256i v1   = kVec;
        __m256i v2   = _mm256_mullo_epi32(kVec, two);
        _mm256_store_si256(reinterpret_cast<__m256i *>(prod1), v1);
        _mm256_store_si256(reinterpret_cast<__m256i *>(prod2), v2);

#pragma omp simd
        for (int i = 0; i < batchSize; ++i) {
            int p1 = prod1[i];
            int p2 = prod2[i];
            // quick length check
            int concat = concatProducts(p1, p2);
            if (concat < 100000000 || concat > 999999999) continue;
            // mask-based pandigital check
            if (isPandigitalMask(p1, p2) && concat > result.maxVal) {
                result.maxVal = concat;
                result.bestK = kArr[i];
            }
        }
    }

    CalcResult calc() {
        constexpr int MAX_K = 9999;
        CalcResult result = {0, 0, 2}; // Initialize with maxVal=0, bestK=0, bestN=2

        // Process k in batches of 8
        for (int k = 1; k <= MAX_K; k += 8) {
            int batchSize = std::min(8, MAX_K - k + 1);
            processBatch(k, batchSize, result);
        }

        return result;
    }
}