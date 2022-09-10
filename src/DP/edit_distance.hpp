#ifndef ALGORITHM_EDIT_DISTANCE_HPP
#define ALGORITHM_EDIT_DISTANCE_HPP 1

#include <algorithm>  // for min().
#include <vector>

namespace algorithm {

// 編集距離 (edit distance)．引数はSTLのシーケンスコンテナ．O(|A|*|B|).
template <class Class>
int edit_distance(const Class &a, const Class &b) {
    const int n = a.size(), m = b.size();
    std::vector<std::vector<int> > dp(n + 1, std::vector<int>(m + 1));  // dp[i][j]:=(a[:i]とb[:j]の編集距離).
    dp[0][0] = 0;
    for(int i = 1; i <= n; ++i) dp[i][0] = i;
    for(int j = 1; j <= m; ++j) dp[0][j] = j;
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= m; ++j) dp[i][j] = std::min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + (a[i - 1] == b[j - 1] ? 0 : 1)});
    return dp[n][m];
}

}  // namespace algorithm

#endif  // ALGORITHM_EDIT_DISTANCE_HPP
