#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
static const int MATCH = 1;
static const int MISMATCH = -1;

string s;
string t;
int rho, sigma;

int max3(int a, int b, int c) {
    return max(a, max(b, c));
}

int getMatchScore(char a, char b) {
    return a == b ? MATCH : MISMATCH;
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
    cout << main[sLen][tLen];
}


int main() {
    cin >> s >> t >> rho >> sigma;
    align();
}
