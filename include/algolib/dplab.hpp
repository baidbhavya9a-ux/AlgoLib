#ifndef ALGOLIB_DPLAB_HPP
#define ALGOLIB_DPLAB_HPP

#include <string>
#include <cstdint>
#include <cstddef>

namespace algolib {

// Fibonacci Result
struct FibResult {
    uint64_t n;
    uint64_t result;
    uint64_t* sequence;
    std::string timeComplexity;
    std::string spaceComplexity;
    std::string explanation;
};

// 0/1 Knapsack Result
struct KnapsackResult {
    int capacity;
    int itemCount;
    int* weights;
    int* values;
    int** dpTable; // (itemCount + 1) x (capacity + 1)
    int maxValue;
    int* selectedItems;
    int selectedCount;
    int totalWeightUsed;
    std::string timeComplexity;
    std::string spaceComplexity;
    std::string explanation;
};

// Longest Common Subsequence Result
struct LCSResult {
    std::string strA;
    std::string strB;
    int** dpTable; // (lenA + 1) x (lenB + 1)
    int lcsLength;
    std::string lcsString;
    std::string timeComplexity;
    std::string spaceComplexity;
    std::string explanation;
};

// Coin Change Result
struct CoinChangeResult {
    int targetAmount;
    int coinCount;
    int* coins;
    int* dpTable; // size targetAmount + 1
    int minCoins;
    int* selectedCoins;
    int selectedCount;
    bool isPossible;
    std::string timeComplexity;
    std::string spaceComplexity;
    std::string explanation;
};

// DP Lab Engine
class DPLab {
public:
    static FibResult computeFibonacci(int n);
    static void freeFibResult(FibResult& res);

    static KnapsackResult solveKnapsack(int capacity, const int* weights, const int* values, int count);
    static void freeKnapsackResult(KnapsackResult& res);

    static LCSResult solveLCS(const std::string& strA, const std::string& strB);
    static void freeLCSResult(LCSResult& res);

    static CoinChangeResult solveCoinChange(int amount, const int* coins, int count);
    static void freeCoinChangeResult(CoinChangeResult& res);
};

} // namespace algolib

#endif // ALGOLIB_DPLAB_HPP

