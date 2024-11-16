#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

static const int BLOSUM62[20][20] = {
    {4, 0, -2, -1, -2, 0, -2, -1, -1, -1, -1, -2, -1, -1, -1, 1, 0, 0, -3, -2},
    {0, 9, -3, -4, -2, -3, -3, -1, -3, -1, -1, -3, -3, -3, -3, -1, -1, -1, -2, -2,},
    {-2, -3, 6, 2, -3, -1, -1, -3, -1, -4, -3, 1, -1, 0, -2, 0, -1, -3, -4, -3},
    {-1, -4, 2, 5, -3, -2, 0, -3, 1, -3, -2, 0, -1, 2, 0, 0, -1, -2, -3, -2},
    {-2, -2, -3, -3, 6, -3, -1, 0, -3, 0, 0, -3, -4, -3, -3, -2, -2, -1, 1, 3},
    {0, -3, -1, -2, -3, 6, -2, -4, -2, -4, -3, 0, -2, -2, -2, 0, -2, -3, -2, -3},
    {-2, -3, -1, 0, -1, -2, 8, -3, -1, -3, -2, 1, -2, 0, 0, -1, -2, -3, -2, 2},
    {-1, -1, -3, -3, 0, -4, -3, 4, -3, 2, 1, -3, -3, -3, -3, -2, -1, 3, -3, -1},
    {-1, -3, -1, 1, -3, -2, -1, -3, 5, -2, -1, 0, -1, 1, 2, 0, -1, -2, -3, -2},
    {-1, -1, -4, -3, 0, -4, -3, 2, -2, 4, 2, -3, -3, -2, -2, -2, -1, 1, -2, -1},
    {-1, -1, -3, -2, 0, -3, -2, 1, -1, 2, 5, -2, -2, 0, -1, -1, -1, 1, -1, -1},
    {-2, -3, 1, 0, -3, 0, 1, -3, 0, -3, -2, 6, -2, 0, 0, 1, 0, -3, -4, -2},
    {-1, -3, -1, -1, -4, -2, -2, -3, -1, -3, -2, -2, 7, -1, -2, -1, -1, -2, -4, -3},
    {-1, -3, 0, 2, -3, -2, 0, -3, 1, -2, 0, 0, -1, 5, 1, 0, -1, -2, -2, -1},
    {-1, -3, -2, 0, -3, -2, 0, -3, 2, -2, -1, 0, -2, 1, 5, -1, -1, -3, -3, -2},
    {1, -1, 0, 0, -2, 0, -1, -2, 0, -2, -1, 1, -1, 0, -1, 4, 1, -2, -3, -2},
    {0, -1, -1, -1, -2, -2, -2, -1, -1, -1, -1, 0, -1, -1, -1, 1, 5, 0, -2, -2},
    {0, -1, -3, -2, -1, -3, -3, 3, -2, 1, 1, -3, -2, -2, -3, -2, 0, 4, -3, -1},
    {-3, -2, -4, -3, 1, -2, -2, -3, -3, -2, -1, -4, -4, -2, -3, -3, -2, -3, 11, 2},
    {-2, -2, -3, -2, 3, -3, 2, -1, -2, -1, -1, -2, -3, -1, -2, -2, -2, -1, 2, 7}
};

unordered_map<char, int> acidIdx = {
    {'A', 0},
    {'C', 1},
    {'D', 2},
    {'E', 3},
    {'F', 4},
    {'G', 5},
    {'H', 6},
    {'I', 7},
    {'K', 8},
    {'L', 9},
    {'M', 10},
    {'N', 11},
    {'P', 12},
    {'Q', 13},
    {'R', 14},
    {'S', 15},
    {'T', 16},
    {'V', 17},
    {'W', 18},
    {'Y', 19}
};

static constexpr int GAP = -5;

string s;
string t;

int max3(int a, int b, int c) {
    return max(a, max(b, c));
}

int getMatchScore(char a, char b) {
    return BLOSUM62[acidIdx[a]][acidIdx[b]];
}

pair<string, string> recover(vector<vector<int> > &score, int i, int j) {
    string as;
    string at;
    while (i || j) {
        if (i && j && score[i][j] == score[i - 1][j - 1] + getMatchScore(s[i - 1], t[j - 1])) {
            as += s[--i];
            at += t[--j];
        } else if (i && score[i][j] == score[i - 1][j] + GAP) {
            as += s[--i];
            at += '-';
        } else {
            as += '-';
            at += t[--j];
        }
    }
    reverse(as.begin(), as.end());
    reverse(at.begin(), at.end());
    return {as, at};
}

void align() {
    const int sLen = s.length();
    const int tLen = t.length();
    vector<vector<int> > score(sLen + 1, vector<int>(tLen + 1, 0));
    for (int i = 0; i <= sLen; i++) {
        score[i][0] = GAP * i;
    }
    for (int j = 0; j <= tLen; j++) {
        score[0][j] = GAP * j;
    }
    for (int i = 0; i ^ sLen; i++) {
        for (int j = 0; j ^ tLen; j++) {
            score[i + 1][j + 1] =
                    max3(
                        score[i][j] + getMatchScore(s[i], t[j]),
                        score[i][j + 1] + GAP,
                        score[i + 1][j] + GAP
                    );
        }
    }
    const pair<string, string> res = recover(score, sLen, tLen);
    cout << res.first << "\n" << res.second;
}

int main() {
    cin >> s;
    cin >> t;
    align();
}
