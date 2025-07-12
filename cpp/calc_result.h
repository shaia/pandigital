/**
 * @file calc_result.h
 * @brief Definition of calculation result structure
 */

#pragma once

namespace impl {
    /**
     * @struct CalcResult
     * @brief Contains the result of pandigital number calculation
     * 
     * @member maxVal Largest pandigital number found
     * @member bestK Value of k that produces maxVal
     * @member bestN Multiplier used (always 2 in current implementation)
     */
    struct CalcResult {
        int maxVal;  ///< Largest pandigital number found
        int bestK;   ///< Value of k that produces maxVal
        int bestN;   ///< Multiplier (fixed at 2)
    };
}