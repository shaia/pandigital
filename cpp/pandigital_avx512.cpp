/**
 * @file pandigital_avx512.cpp
 * @brief AVX-512 optimized implementation
 * 
 * Features:
 * 1. 512-bit vector operations
 * 2. Mask operations for comparisons
 * 3. Dynamic shift computation
 * 4. Processes 16 numbers in parallel
 * 
 * Requirements:
 * - CPU with AVX-512F support
 * - Optional: AVX-512BW, AVX-512DQ, AVX-512VL
 * - 64-byte aligned memory
 * 
 * Optimizations:
 * - Mask-based comparisons
 * - Blend operations for conditional moves
 * - Optimized digit counting and validation
 * 
 * Performance characteristics:
 * - Maximum parallelism: 16 integers
 * - Memory usage: 192 bytes aligned buffers
 * - Reduced branch operations
 */
#include <immintrin.h>
#include <iostream>
#include <algorithm>
#include "calc_result.h"

namespace impl::avx512 {
    // Precomputed pandigital mask full bits for 1–9
    constexpr int FULL_MASK = 0x3FE;

    // Check mask-based pandigital for scalar fallback
    inline bool maskPandigitalScalar(int value, int p1, int p2) {
        int mask = 0;
        while (p1) { int d = p1 % 10; if (d == 0 || (mask & (1<<d))) return false; mask |= 1<<d; p1/=10; }
        while (p2) { int d = p2 % 10; if (mask & (1<<d)) return false; mask |= 1<<d; p2/=10; }
        return mask == FULL_MASK && value >= 100000000 && value <= 999999999;
    }

    CalcResult calc() {
        constexpr int MAX_K = 9999;
        constexpr int BATCH = 16;                // AVX-512 16 lanes
        alignas(64) int kArr[BATCH];
        alignas(64) int concatArr[BATCH];

        CalcResult result = {0, 0, 2}; // Initialize with maxVal=0, bestK=0, bestN=2

        for (int k = 1; k <= MAX_K; k += BATCH) {
            int bs = std::min(BATCH, MAX_K - k + 1);
            for (int i = 0; i < bs; ++i) kArr[i] = k + i;
            for (int i = bs; i < BATCH; ++i) kArr[i] = 0;

            __m512i kVec    = _mm512_load_epi32(kArr);
            __m512i two     = _mm512_set1_epi32(2);
            __m512i p1      = kVec;
            __m512i p2      = _mm512_mullo_epi32(kVec, two);

            // Compute digit-shift for p2.
            __m512i ten1    = _mm512_set1_epi32(10);
            __m512i ten2    = _mm512_set1_epi32(100);
            __m512i ten3    = _mm512_set1_epi32(1000);
            __m512i ten4    = _mm512_set1_epi32(10000);

            __mmask16 m1 = _mm512_cmp_epi32_mask(p2, ten1, _MM_CMPINT_LT);
            __mmask16 m2 = _mm512_cmp_epi32_mask(p2, ten2, _MM_CMPINT_LT);
            __mmask16 m3 = _mm512_cmp_epi32_mask(p2, ten3, _MM_CMPINT_LT);
            __mmask16 m4 = _mm512_cmp_epi32_mask(p2, ten4, _MM_CMPINT_LT);

            __m512i shift = _mm512_set1_epi32(100000);
            shift = _mm512_mask_blend_epi32(m4, shift, _mm512_set1_epi32(10000));
            shift = _mm512_mask_blend_epi32(m3, shift, _mm512_set1_epi32(1000));
            shift = _mm512_mask_blend_epi32(m2, shift, _mm512_set1_epi32(100));
            shift = _mm512_mask_blend_epi32(m1, shift, _mm512_set1_epi32(10));

            __m512i concatVec = _mm512_add_epi32(_mm512_mullo_epi32(p1, shift), p2);
            _mm512_store_epi32(concatArr, concatVec);

            for (int i = 0; i < bs; ++i) {
                int val = concatArr[i];
                int p1s = kArr[i];
                int p2s = kArr[i] * 2;
                if (maskPandigitalScalar(val, p1s, p2s) && val > result.maxVal) {
                    result.maxVal = val;
                    result.bestK = p1s;
                }
            }
        }

        return result;
    }
}