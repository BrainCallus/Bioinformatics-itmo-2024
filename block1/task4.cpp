#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
static const int MATCH = 1;
static const int MISMATCH = -1;

string s;
string t;
int rho, sigma;
string as, at;

int max3(int a, int b, int c) {
    return max(a, max(b, c));
}

int getMatchScore(char a, char b) {
    return a == b ? MATCH : MISMATCH;
}


char inUpperRecover(int &i, int &j, const vector<vector<int> > &upper) {
    as += '-';
    at += t[--j];
    if (upper[i][j + 1] == upper[i][j] + sigma) {
        return 'u';
    } else {
        return 'm';
    }
}

char inLowerRecover(int &i, const int &j, const vector<vector<int> > &lower) {
    as += s[--i];
    at += '-';
    if (lower[i + 1][j] == lower[i][j] + sigma) {
        return 'l';
    } else {
        return 'm';
    }
}

char inMainRecover(int &i, int &j, vector<vector<int> > &main, vector<vector<int> > &upper,
                   vector<vector<int> > &lower) {
    if (i && j && (main[i][j] == main[i - 1][j - 1] + MATCH && s[i - 1] == t[j - 1] ||
                   main[i][j] == main[i - 1][j - 1] + MISMATCH && s[i - 1] != t[j - 1])) {
        as += s[--i];
        at += t[--j];
        return 'm';
    }
    if (i && main[i][j] == lower[i][j]) {
        return inLowerRecover(i, j, lower);
    }
    return inUpperRecover(i, j, upper);
}

pair<string, string>
recover(vector<vector<int> > &main, vector<vector<int> > &upper, vector<vector<int> > &lower, int i, int j) {
    char flag = 'm';
    while (i || j) {
        switch (flag) {
            case 'm': {
                flag = inMainRecover(i, j, main, upper, lower);
                break;
            }
            case 'u': {
                flag = inUpperRecover(i, j, upper);
                break;
            }
            default: {
                flag = inLowerRecover(i, j, lower);
            }
        }
    }
    reverse(as.begin(), as.end());
    reverse(at.begin(), at.end());
    return {as, at};
}

void align() {
    int sLen = s.length();
    int tLen = t.length();
    vector<vector<int> > main(sLen + 1, vector<int>(tLen + 1, 0));
    vector<vector<int> > upper(sLen + 1, vector<int>(tLen + 1, 0));
    vector<vector<int> > lower(sLen + 1, vector<int>(tLen + 1, 0));
    main[1][0] = main[0][1] = rho + sigma;
    upper[1][0] = upper[0][1] = rho + sigma;
    lower[1][0] = lower[0][1] = rho + sigma;
    for (int i = 1; i ^ sLen; i++) {
        main[i + 1][0] = main[i][0] + sigma;
        upper[i + 1][0] = upper[i][0] + sigma;
        lower[i + 1][0] = lower[i][0] + sigma;
    }
    for (int j = 1; j ^ tLen; j++) {
        main[0][j + 1] = main[0][j] + sigma;
        upper[0][j + 1] = upper[0][j] + sigma;
        lower[0][j + 1] = lower[0][j] + sigma;
    }
    for (int i = 0; i ^ sLen; i++) {
        for (int j = 0; j ^ tLen; j++) {
            upper[i + 1][j + 1] = max(upper[i + 1][j] + sigma, main[i + 1][j] + rho + sigma);
            lower[i + 1][j + 1] = max(lower[i][j + 1] + sigma, main[i][j + 1] + rho + sigma);
            main[i + 1][j + 1] = max3(main[i][j] + getMatchScore(s[i], t[j]), upper[i + 1][j + 1],
                                      lower[i + 1][j + 1]);
        }
    }
    pair<string, string> res = recover(main, upper, lower, sLen, tLen);
    cout << res.first << "\n" << res.second;
}


int main() {
    cin >> s >> t >> rho >> sigma;
    align();
}
