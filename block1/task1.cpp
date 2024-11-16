#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
static const int MATCH = 1;
static const int MISMATCH = -1;
static const int GAP = -2;

string s;
string t;

int max3(int a, int b, int c) {
    return max(a, max(b, c));
}

int getMatchScore(char a, char b) {
    return a == b ? MATCH : MISMATCH;
}

pair<string, string> recover(vector<vector<int> > &score, int i, int j) {
    string as;
    string at;
    while (i || j) {
        if (i && j &&
            (score[i][j] == score[i - 1][j - 1] + MATCH && s[i - 1] == t[j - 1] ||
             score[i][j] == score[i - 1][j - 1] + MISMATCH && s[i - 1] != t[j - 1])) {
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
    int sLen = s.length();
    int tLen = t.length();
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
    pair<string, string> res = recover(score, sLen, tLen);
    cout << res.first << "\n" << res.second;
}

int main() {
    cin >> s;
    cin >> t;
    align();
}
