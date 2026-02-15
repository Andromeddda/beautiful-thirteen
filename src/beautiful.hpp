#include <cinttypes>
#include <vector>
#include <array>

// Let's compute the number of "beautiful" numbers 
//      NOTE: A "beautiful" number is a 13-digit number in base 13 
//      (with leading zeros allowed), such that the sum of its first 
//      6 digits equals to the sum of its last 6 digits.
//
// For every possible digit-sum S in base 13
// from 0 = 0+0+0+0+0+0 to 72 = 12+12+12+12+12+12 = C+C+C+C+C+C
// let N(S) be the number of 6-digit base-13 sequences whose
// digits sum to exactly S.

// Each "beautiful" number with half-sum S is formed
// by choosing:
//   - a left  half with digit sum S   -  N(S) ways
//   - a middle digit (0..12)          -  13  ways
//   - a right half with digit sum S   -  N(S) ways
//

// Therefore, the total number of "beautiful" numbers is calculated like this:
//
//      total := 0
//      FOR S = 1..72:
//          total += 13*N(S)*N(S)
//

// N(S) is computed via inclusion-exclusion over the constraint 
// 0 < x < 13 for each of the 6 digits: 

// N(S) = [solutions with no constraints] - SUM |Ai| + SUM |Ai AND Aj| - ... +  SUM |A1 AND A2 AND ... AND A6|
// Where Ai is the set of solutions where the i-th digit is >= 13 

// [solutions with no constraints]  = C(S+5, 5)
// |Ai|                             = C(S-13+5, 5) 
// |Ai AND Aj|                      = C(S-2*13+5, 5)
// ...
// |A1 AND A2 AND ... AND A6|       = C(S-6*13+5, 5)

// because solution with constraint x_i >= 13 
// is equivalent to solution with x_i' = x_i - 13 >= 0, and S' = S - 13

// NOTE:
//      C(n, k) = 0                 if n < k or k < 0
//      C(n, k) = n!/(k!*(n-k)!)    otherwise 


namespace beautiful_numbers {
    
    constexpr int kDigits   = 6;
    constexpr int kBase     = 13;
    constexpr int kMaxSum   = (kBase - 1) * kDigits; // 72

    // C(n, 5) for small non-negative n
    [[nodiscard]] constexpr int64_t binomial_n_5(int n_) noexcept
    {
        if (n_ < 5) [[unlikely]] {
            return 0;
        }

        const auto m = static_cast<int64_t>(n_);
        return (m * (m - 1) * (m - 2) * (m - 3) * (m - 4)) / 120;
    }

    // Precomputed C(6, k) for k = 0..6 
    constexpr std::array<int64_t, kDigits + 1> kBinom6 = {1, 6, 15, 20, 15, 6, 1};

    // N(S) computed via inclusion-exclusion
    [[nodiscard]] constexpr int64_t ways_for_sum(int sum_) noexcept
    {
        int64_t result = 0;
        int     sign   = 1;

        for (int k = 0; k <= kDigits; ++k) {

            const int arg = sum_ - kBase * k + 5; // S - k*13 + 5

            // if arg < 5, all next terms will be zero as well 
            if (arg < 5) [[unlikely]] {
                break; 
            }

            // (-1)^k * C(6, k) * C(S - k*13 + 5, 5)
            result += sign * kBinom6[k] * binomial_n_5(arg);

            // inclusion-exclusion alternates signs
            sign = -sign; 
        }

        return result;
    }

    // Compile-time lookup table: N(S) for S in [0, kMaxSum]
    [[nodiscard]] consteval std::array<int64_t, kMaxSum + 1> build_ways_lookup_table() noexcept
    {
        std::array<int64_t, kMaxSum + 1> table{};
    
        for (int s = 0; s <= kMaxSum; ++s) {
            table[s] = ways_for_sum(s);
        }
        return table;
    }

    constexpr std::array<int64_t, kMaxSum + 1> kWays = build_ways_lookup_table();

    [[nodiscard]] int64_t count_beautiful_numbers() noexcept
    {
        int64_t total = 0;

        for (int s = 0; s <= kMaxSum; ++s) {
            total += kBase * kWays[s] * kWays[s];
        }

        return total;
    }

} // namespace beautiful_numbers