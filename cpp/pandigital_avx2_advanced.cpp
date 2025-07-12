#include <immintrin.h>
#include <iostream>
#include <algorithm>
#include "calc_result.h"

namespace impl::avx2_advanced {
    constexpr int FULL_MASK = 0x3FE;
    
    // AVX2 constants
    static const __m256i V_TWO = _mm256_set1_epi32(2);
    static const __m256i V_TEN = _mm256_set1_epi32(10);
    static const __m256i V_100M = _mm256_set1_epi32(100000000);
    static const __m256i V_1B = _mm256_set1_epi32(1000000000);
    static const __m256i V_FULL_MASK = _mm256_set1_epi32(FULL_MASK);
    
    // Shift constants
    static const __m256i V_TEN1 = _mm256_set1_epi32(10);
    static const __m256i V_TEN2 = _mm256_set1_epi32(100);
    static const __m256i V_TEN3 = _mm256_set1_epi32(1000);
    static const __m256i V_TEN4 = _mm256_set1_epi32(10000);
    static const __m256i V_SHIFT_DEFAULT = _mm256_set1_epi32(100000);

    inline __m256i compute_digit_mask(__m256i num) {
        __m256i mask = _mm256_setzero_si256();
        __m256i v = num;
        for (int i = 0; i < 5; ++i) {
            // Get the remainder when divided by 10
            __m256i quotient = _mm256_div_epi32(v, V_TEN);
            __m256i product = _mm256_mullo_epi32(quotient, V_TEN);
            __m256i remainder = _mm256_sub_epi32(v, product);
            
            // Create a bit mask for this digit
            __m256i bit = _mm256_sllv_epi32(_mm256_set1_epi32(1), remainder);
            mask = _mm256_or_si256(mask, bit);
            
            v = quotient;
        }
        return mask;
    }

    CalcResult calc() {
        constexpr int MAX_K = 9999;
        constexpr int BATCH = 8;  // AVX2 has 8 32-bit lanes
        alignas(32) int kArr[BATCH];

        CalcResult result = {0, 0, 2};

        for (int k = 1; k <= MAX_K; k += BATCH) {
            int concatArr[BATCH];
            int bs = std::min(BATCH, MAX_K - k + 1);
            for (int i = 0; i < bs; ++i) kArr[i] = k + i;
            for (int i = bs; i < BATCH; ++i) kArr[i] = 0;

            // Load and compute products
            __m256i kVec = _mm256_load_si256(reinterpret_cast<__m256i *>(kArr));
            __m256i p1 = kVec;
            __m256i p2 = _mm256_mullo_epi32(kVec, V_TWO);

            // Compute shift amount based on the number of digits in p2.
            __m256i shift = V_SHIFT_DEFAULT;
            __m256i cmp = _mm256_cmpgt_epi32(V_TEN1, p2);
            shift = _mm256_blendv_epi8(shift, V_TEN1, cmp);
            cmp = _mm256_cmpgt_epi32(V_TEN2, p2);
            shift = _mm256_blendv_epi8(shift, V_TEN2, cmp);
            cmp = _mm256_cmpgt_epi32(V_TEN3, p2);
            shift = _mm256_blendv_epi8(shift, V_TEN3, cmp);
            cmp = _mm256_cmpgt_epi32(V_TEN4, p2);
            shift = _mm256_blendv_epi8(shift, V_TEN4, cmp);

            // Concatenate numbers
            __m256i concat = _mm256_add_epi32(
                _mm256_mullo_epi32(p1, shift), p2);
            _mm256_store_si256(reinterpret_cast<__m256i *>(concatArr), concat);

            // Compute digit masks
            __m256i mask1 = compute_digit_mask(p1);
            __m256i mask2 = compute_digit_mask(p2);
            __m256i maskVec = _mm256_or_si256(mask1, mask2);

            // Compare masks and range check
            __m256i validMask = _mm256_cmpeq_epi32(maskVec, V_FULL_MASK);
            __m256i rangeMask = _mm256_and_si256(
                _mm256_cmpgt_epi32(V_1B, concat),
                _mm256_cmpgt_epi32(concat, V_100M)
            );
            validMask = _mm256_and_si256(validMask, rangeMask);

            // Extract mask and process valid results
            int mask = _mm256_movemask_epi8(validMask);
            if (mask) {
                for (int i = 0; i < bs; ++i) {
                    if (mask & (1 << (i * 4))) {
                        int val = concatArr[i];
                        if (val > result.maxVal) {
                            result.maxVal = val;
                            result.bestK = kArr[i];
                        }
                    }
                }
            }
        }

        return result;
    }
}