#include <iostream>
#include <vector>

using namespace std;

string s;

int getMatchMismatchScore(int i, int j) {
    switch (s[i]) {
        case 'A':
            return s[j] == 'U';
        case 'C':
            return s[j] == 'G';
        case 'G':
            return s[j] == 'C';
        case 'U':
            return s[j] == 'A';
        default:
            return 0;
    }
}

void pairBases() {
    vector<vector<int> > dp(s.length());
    for (int i = 0; i < s.length(); i++) {
        dp[i] = vector<int>(s.length() - i, 0); // tiny mem optim
    }

    for (int i = 0; i < s.length() - 1; i++) {
        for (int j = i + 2; j < s.length(); j++) {
            int row = j - i - 2;
            int col = j;
            dp[row][col - row] = dp[row + 1][col - 1 - (row + 1)] + getMatchMismatchScore(row, col);
            for (int k = row; k < col; k++) {
                dp[row][col - row] = max(dp[row][col - row], dp[row][k - row] + dp[k + 1][col - (k + 1)]);
            }
        }
    }
    cout << dp[0][s.length() - 1];
}

int main() {
    cin >> s;
    pairBases();
}
