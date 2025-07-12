/**
 * @file pandigital_simple.cpp
 * @brief Simple sequential implementation of pandigital number finder
 * 
 * This implementation serves as:
 * 1. Reference implementation for correctness
 * 2. Baseline for performance comparison
 * 3. Fallback for systems without SIMD support
 * 
 * Algorithm complexity: O(n * log n)
 * Memory usage: O(1)
 */

#include <string>
#include "calc_result.h"

namespace impl::simple {
    /**
     * @brief Checks if a string represents a 1-9 pandigital number
     * @param s String to check
     * @return true if string is pandigital
     * 
     * A number is pandigital if it:
     * - Contains exactly 9 digits
     * - Uses each digit 1-9 exactly once
     * - Does not contain 0
     */
    bool isPandigital(const std::string& s) {
        if (s.length() != 9) return false;
        bool used[10] = {false};
        for (char c : s) {
            int digit = c - '0';
            if (digit == 0 || used[digit]) return false;
            used[digit] = true;
        }
        for (int i = 1; i <= 9; ++i) {
            if (!used[i]) return false;
        }
        return true;
    }

    /**
     * @brief Calculates the largest pandigital number from concatenated k and 2k
     * @return CalcResult containing the maximum value and corresponding k
     */
    CalcResult calc() {
        CalcResult result = {0, 0, 2}; // maxVal=0, bestK=0, bestN=2

        for (int k = 1; k < 10000; ++k) {
            int p1 = k;
            int p2 = k * 2;
            std::string concat = std::to_string(p1) + std::to_string(p2);
            
            if (concat.length() != 9) continue;
            
            if (isPandigital(concat)) {
                int val = std::stoi(concat);
                if (val > result.maxVal) {
                    result.maxVal = val;
                    result.bestK = k;
                }
            }
        }

        return result;
    }
}