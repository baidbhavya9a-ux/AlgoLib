#include "algolib/dplab.hpp"

#include <sstream>
#include <iomanip>
#include <algorithm>

namespace algolib {

// ==================================================
// 1. FIBONACCI
// ==================================================

FibResult DPLab::computeFibonacci(int n) {
    FibResult res;
    res.n = (n < 0) ? 0 : static_cast<uint64_t>(n);
    res.timeComplexity = "O(N) - Linear Time Tabulation";
    res.spaceComplexity = "O(N) - Tabulation Array (or O(1) iterative)";

    if (n < 0) {
        res.result = 0;
        res.sequence = new uint64_t[1]{0};
        res.explanation = "Invalid input: n must be non-negative.";
        return res;
    }

    res.sequence = new uint64_t[n + 2];
    res.sequence[0] = 0;
    res.sequence[1] = 1;

    for (int i = 2; i <= n; ++i) {
        res.sequence[i] = res.sequence[i - 1] + res.sequence[i - 2];
    }

    res.result = res.sequence[n];

    std::ostringstream ss;
    ss << "=== DYNAMIC PROGRAMMING: FIBONACCI ===\n";
    ss << "Target Term: F(" << n << ")\n";
    ss << "Calculated Value: " << res.result << "\n\n";
    ss << "Recurrence Relation: F(i) = F(i-1) + F(i-2), with F(0)=0, F(1)=1\n";
    ss << "Time Complexity: " << res.timeComplexity << "\n";
    ss << "Space Complexity: " << res.spaceComplexity << "\n\n";
    ss << "Sequence Table:\n";
    for (int i = 0; i <= n; ++i) {
        ss << "F(" << i << ") = " << res.sequence[i];
        if (i < n) ss << ",  ";
        if ((i + 1) % 5 == 0) ss << "\n";
    }
    ss << "\n";

    res.explanation = ss.str();
    return res;
}

void DPLab::freeFibResult(FibResult& res) {
    delete[] res.sequence;
    res.sequence = nullptr;
}


// ==================================================
// 2. 0/1 KNAPSACK
// ==================================================

KnapsackResult DPLab::solveKnapsack(int capacity, const int* weights, const int* values, int count) {
    KnapsackResult res;
    res.capacity = (capacity < 0) ? 0 : capacity;
    res.itemCount = (count < 0) ? 0 : count;
    res.maxValue = 0;
    res.totalWeightUsed = 0;
    res.selectedCount = 0;
    res.selectedItems = nullptr;
    res.dpTable = nullptr;
    res.weights = nullptr;
    res.values = nullptr;
    res.timeComplexity = "O(N * W) - Pseudo-polynomial Time";
    res.spaceComplexity = "O(N * W) - 2D DP State Table";

    if (count <= 0 || capacity <= 0 || !weights || !values) {
        res.explanation = "Empty knapsack or no valid items provided.";
        return res;
    }

    res.weights = new int[count];
    res.values = new int[count];
    for (int i = 0; i < count; ++i) {
        res.weights[i] = weights[i];
        res.values[i] = values[i];
    }

    // Allocate 2D DP Table: (count + 1) x (capacity + 1)
    res.dpTable = new int*[count + 1];
    for (int i = 0; i <= count; ++i) {
        res.dpTable[i] = new int[capacity + 1];
        for (int w = 0; w <= capacity; ++w) {
            res.dpTable[i][w] = 0;
        }
    }

    // Populate DP Table
    for (int i = 1; i <= count; ++i) {
        int wt = weights[i - 1];
        int val = values[i - 1];
        for (int w = 0; w <= capacity; ++w) {
            if (wt <= w) {
                int includeItem = val + res.dpTable[i - 1][w - wt];
                int excludeItem = res.dpTable[i - 1][w];
                res.dpTable[i][w] = (includeItem > excludeItem) ? includeItem : excludeItem;
            } else {
                res.dpTable[i][w] = res.dpTable[i - 1][w];
            }
        }
    }

    res.maxValue = res.dpTable[count][capacity];

    // Backtrack to find selected items
    int* tempSelected = new int[count];
    int selIdx = 0;
    int currW = capacity;

    for (int i = count; i > 0 && currW > 0; --i) {
        if (res.dpTable[i][currW] != res.dpTable[i - 1][currW]) {
            tempSelected[selIdx++] = i - 1; // item 0-based index
            res.totalWeightUsed += weights[i - 1];
            currW -= weights[i - 1];
        }
    }

    // Reverse to chronological order
    res.selectedCount = selIdx;
    res.selectedItems = new int[selIdx > 0 ? selIdx : 1];
    for (int i = 0; i < selIdx; ++i) {
        res.selectedItems[i] = tempSelected[selIdx - 1 - i];
    }
    delete[] tempSelected;

    std::ostringstream ss;
    ss << "=== 0/1 KNAPSACK OPTIMIZATION ===\n";
    ss << "Knapsack Capacity: " << capacity << " | Number of Items: " << count << "\n";
    ss << "Maximum Achievable Value: " << res.maxValue << "\n";
    ss << "Total Weight Utilized: " << res.totalWeightUsed << " / " << capacity << "\n\n";
    ss << "Selected Items:\n";
    ss << std::string(45, '-') << "\n";
    ss << "Item #      Weight      Value\n";
    ss << std::string(45, '-') << "\n";

    for (int i = 0; i < res.selectedCount; ++i) {
        int idx = res.selectedItems[i];
        ss << "Item " << (idx + 1) << "       " << weights[idx] << "           " << values[idx] << "\n";
    }

    ss << "\nRecurrence:\n";
    ss << "dp[i][w] = max(dp[i-1][w], val[i-1] + dp[i-1][w - wt[i-1]])\n";
    ss << "Time Complexity: " << res.timeComplexity << "\n";
    ss << "Space Complexity: " << res.spaceComplexity << "\n";

    res.explanation = ss.str();
    return res;
}

void DPLab::freeKnapsackResult(KnapsackResult& res) {
    if (res.dpTable) {
        for (int i = 0; i <= res.itemCount; ++i) {
            delete[] res.dpTable[i];
        }
        delete[] res.dpTable;
        res.dpTable = nullptr;
    }
    delete[] res.weights;
    delete[] res.values;
    delete[] res.selectedItems;
    res.weights = nullptr;
    res.values = nullptr;
    res.selectedItems = nullptr;
}


// ==================================================
// 3. LONGEST COMMON SUBSEQUENCE (LCS)
// ==================================================

LCSResult DPLab::solveLCS(const std::string& strA, const std::string& strB) {
    LCSResult res;
    res.strA = strA;
    res.strB = strB;
    res.dpTable = nullptr;
    res.lcsLength = 0;
    res.timeComplexity = "O(M * N) - Matrix Tabulation";
    res.spaceComplexity = "O(M * N) - 2D DP Table";

    int m = static_cast<int>(strA.length());
    int n = static_cast<int>(strB.length());

    // Allocate 2D DP Table
    res.dpTable = new int*[m + 1];
    for (int i = 0; i <= m; ++i) {
        res.dpTable[i] = new int[n + 1];
        for (int j = 0; j <= n; ++j) {
            res.dpTable[i][j] = 0;
        }
    }

    // Populate DP Table
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (strA[i - 1] == strB[j - 1]) {
                res.dpTable[i][j] = 1 + res.dpTable[i - 1][j - 1];
            } else {
                int up = res.dpTable[i - 1][j];
                int left = res.dpTable[i][j - 1];
                res.dpTable[i][j] = (up > left) ? up : left;
            }
        }
    }

    res.lcsLength = res.dpTable[m][n];

    // Reconstruct LCS string by backtracking
    char* lcsChars = new char[res.lcsLength + 1];
    int charIdx = res.lcsLength - 1;
    lcsChars[res.lcsLength] = '\0';

    int i = m;
    int j = n;
    while (i > 0 && j > 0) {
        if (strA[i - 1] == strB[j - 1]) {
            lcsChars[charIdx--] = strA[i - 1];
            --i;
            --j;
        } else if (res.dpTable[i - 1][j] > res.dpTable[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }

    res.lcsString = std::string(lcsChars);
    delete[] lcsChars;

    std::ostringstream ss;
    ss << "=== LONGEST COMMON SUBSEQUENCE (LCS) ===\n";
    ss << "String A: \"" << strA << "\" (Length: " << m << ")\n";
    ss << "String B: \"" << strB << "\" (Length: " << n << ")\n\n";
    ss << "Length of LCS: " << res.lcsLength << "\n";
    ss << "Reconstructed LCS: \"" << res.lcsString << "\"\n\n";
    ss << "Recurrence:\n";
    ss << "If A[i-1] == B[j-1]: dp[i][j] = 1 + dp[i-1][j-1]\n";
    ss << "Else: dp[i][j] = max(dp[i-1][j], dp[i][j-1])\n";
    ss << "Time Complexity: " << res.timeComplexity << "\n";
    ss << "Space Complexity: " << res.spaceComplexity << "\n";

    res.explanation = ss.str();
    return res;
}

void DPLab::freeLCSResult(LCSResult& res) {
    if (res.dpTable) {
        int m = static_cast<int>(res.strA.length());
        for (int i = 0; i <= m; ++i) {
            delete[] res.dpTable[i];
        }
        delete[] res.dpTable;
        res.dpTable = nullptr;
    }
}


// ==================================================
// 4. COIN CHANGE (FEWEST COINS)
// ==================================================

CoinChangeResult DPLab::solveCoinChange(int amount, const int* coins, int count) {
    CoinChangeResult res;
    res.targetAmount = amount;
    res.coinCount = count;
    res.coins = nullptr;
    res.dpTable = nullptr;
    res.selectedCoins = nullptr;
    res.selectedCount = 0;
    res.minCoins = -1;
    res.isPossible = false;
    res.timeComplexity = "O(Amount * Coins) - Bottom-Up Tabulation";
    res.spaceComplexity = "O(Amount) - 1D DP State Array";

    if (amount < 0 || count <= 0 || !coins) {
        res.explanation = "Invalid amount or coin denominations provided.";
        return res;
    }

    res.coins = new int[count];
    for (int i = 0; i < count; ++i)
        res.coins[i] = coins[i];

    const int INF_COINS = 1e9;
    res.dpTable = new int[amount + 1];
    int* coinChoice = new int[amount + 1];

    res.dpTable[0] = 0;
    coinChoice[0] = -1;

    for (int i = 1; i <= amount; ++i) {
        res.dpTable[i] = INF_COINS;
        coinChoice[i] = -1;
    }

    // Dynamic Programming Tabulation
    for (int i = 1; i <= amount; ++i) {
        for (int c = 0; c < count; ++c) {
            int coin = coins[c];
            if (coin <= i && res.dpTable[i - coin] != INF_COINS) {
                if (1 + res.dpTable[i - coin] < res.dpTable[i]) {
                    res.dpTable[i] = 1 + res.dpTable[i - coin];
                    coinChoice[i] = coin;
                }
            }
        }
    }

    if (res.dpTable[amount] < INF_COINS) {
        res.isPossible = true;
        res.minCoins = res.dpTable[amount];

        // Backtrack to extract coin combination
        res.selectedCoins = new int[res.minCoins > 0 ? res.minCoins : 1];
        int curr = amount;
        int idx = 0;
        while (curr > 0 && coinChoice[curr] != -1) {
            int c = coinChoice[curr];
            res.selectedCoins[idx++] = c;
            curr -= c;
        }
        res.selectedCount = idx;
    } else {
        res.isPossible = false;
        res.minCoins = -1;
    }

    delete[] coinChoice;

    std::ostringstream ss;
    ss << "=== COIN CHANGE: MINIMUM COINS OPTIMIZATION ===\n";
    ss << "Target Amount: " << amount << "\n";
    ss << "Available Coin Denominations: [ ";
    for (int i = 0; i < count; ++i)
        ss << coins[i] << " ";
    ss << "]\n\n";

    if (res.isPossible) {
        ss << "STATUS: Solved!\n";
        ss << "Minimum Coins Required: " << res.minCoins << "\n";
        ss << "Exact Coin Combination: ";
        for (int i = 0; i < res.selectedCount; ++i) {
            ss << res.selectedCoins[i];
            if (i + 1 < res.selectedCount) ss << " + ";
        }
        ss << " = " << amount << "\n\n";
    } else {
        ss << "STATUS: Impossible! The target amount cannot be formed with the given coins.\n\n";
    }

    ss << "Recurrence: dp[i] = min(dp[i], 1 + dp[i - coin]) for each coin <= i\n";
    ss << "Time Complexity: " << res.timeComplexity << "\n";
    ss << "Space Complexity: " << res.spaceComplexity << "\n";

    res.explanation = ss.str();
    return res;
}

void DPLab::freeCoinChangeResult(CoinChangeResult& res) {
    delete[] res.coins;
    delete[] res.dpTable;
    delete[] res.selectedCoins;
    res.coins = nullptr;
    res.dpTable = nullptr;
    res.selectedCoins = nullptr;
}

} // namespace algolib

