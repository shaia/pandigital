/**
 * @file pandigital_simd.cpp
 * @brief Base SIMD implementation using AVX2 instructions
 * 
 * Features:
 * 1. Processes 8 numbers in parallel using 256-bit AVX2 registers
 * 2. Uses aligned memory for optimal SIMD performance
 * 3. Hybrid approach: SIMD for multiplication, scalar for validation
 * 
 * Requirements:
 * - CPU with AVX2 support
 * - Aligned memory allocation
 * 
 * Performance characteristics:
 * - Time complexity: O((n/8) * log n)
 * - Memory alignment: 32-byte boundaries
 * - Cache usage: ~96 bytes of aligned buffers
 */
#include <immintrin.h>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include "calc_result.h"

namespace impl::base_simd {
    /**
     * @brief Validates if a character array represents a pandigital number
     * @param s Pointer to 9-character array
     * @return true if number is pandigital
     * 
     * Optimized for performance with:
     * - Direct character array access
     * - Early exit conditions
     * - Boolean array for digit tracking
     */
    bool isPandigital(const char *s) {
        bool seen[10] = {};
        for (int i = 0; i < 9; i++) {
            int d = s[i] - '0';
            if (d <= 0 || d > 9 || seen[d]) return false;
            seen[d] = true;
        }
        return true;
    }

    /**
     * @brief Calculates largest pandigital number using SIMD operations
     * @return CalcResult with maximum value and corresponding k
     * 
     * Implementation steps:
     * 1. Process numbers in batches of 8 using AVX2
     * 2. Perform parallel multiplication
     * 3. Convert results to string and validate
     * 4. Track maximum valid pandigital number
     */
    CalcResult calc() {
        constexpr int MAX_K = 9999;
        CalcResult result = {0, 0, 2}; // Initialize with maxVal=0, bestK=0, bestN=2

        alignas(32) int kArr[8];

        __m256i two = _mm256_set1_epi32(2);

        for (int k = 1; k <= MAX_K; k += 8) {
            int prod2[8];
            int prod1[8];
            int batchSize = std::min(8, MAX_K - k + 1);
            // Prepare k values
            for (int i = 0; i < batchSize; i++) {
                kArr[i] = k + i;
            }
            for (int i = batchSize; i < 8; i++) {
                kArr[i] = 0;
            }

            // Load k values (unaligned loads)
            __m256i kVec = _mm256_loadu_si256(reinterpret_cast<__m256i *>(kArr));

            // Compute k*1 and k*2
            __m256i prodVec1 = kVec;
            __m256i prodVec2 = _mm256_mullo_epi32(kVec, two);

            // Store results back
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(prod1), prodVec1);
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(prod2), prodVec2);

            // Scalar check
            for (int i = 0; i < batchSize; i++) {
                int p1 = prod1[i];
                int p2 = prod2[i];
                char s[10];

                // Using snprintf instead of std::to_string or stringstream for performance
                // in this hot loop. Modern C++ alternatives would involve heap allocations
                // which are significantly slower in this performance-critical section.
                int len = std::snprintf(s, sizeof(s), "%d%d", p1, p2);
                if (len < 0 || len >= sizeof(s)) continue;  // Error or truncation occurred
                if (len != 9) continue;
                if (isPandigital(s)) {
                    int val = std::atoi(s);
                    if (val > result.maxVal) {
                        result.maxVal = val;
                        result.bestK = kArr[i];
                    }
                }
            }
        }

        return result;
    }
}