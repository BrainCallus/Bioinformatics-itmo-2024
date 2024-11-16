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

struct Node {
    int score;
    bool duplicate;
    Node *diagPrev = nullptr;
    pair<Node *, Node *> leftRight;

    explicit Node(int score, bool duplicate) {
        this->score = score;
        this->duplicate = duplicate;
        leftRight.first = nullptr;
        leftRight.second = nullptr;
    }
};

string recover(Node *cur) {
    string ans;
    if (cur->diagPrev) {
        if (cur->score - cur->diagPrev->score) {
            return '(' + recover(cur->diagPrev) + ')';
        }
        return '.' + recover(cur->diagPrev) + '.';
    }
    if (cur->leftRight.first) {
        return recover(cur->leftRight.first) + recover(cur->leftRight.second);
    }
    return cur->duplicate ? ".." : ".";
}

void pairBases() {
    vector<vector<Node *> > dp(s.length());

    for (int i = 0; i < s.length(); i++) {
        dp[i] = vector<Node *>(s.length() - i);
        for (int j = 0; j < s.length() - i; j++) {
            dp[i][j] = new Node(0, j);
        }
    }

    for (int i = 0; i < s.length() - 1; i++) {
        for (int j = i + 2; j < s.length(); j++) {
            int row = j - i - 2;
            int col = j;
            Node *cur = dp[row][col - row];

            cur->score = dp[row + 1][col - 1 - (row + 1)]->score + getMatchMismatchScore(row, col);
            cur->diagPrev = dp[row + 1][col - 1 - (row + 1)];

            for (int k = row; k < col; k++) {
                Node *first_term = dp[row][k - row];
                if (Node *second_term = dp[k + 1][col - (k + 1)];
                    cur->score < first_term->score + second_term->score) {
                    cur->score = first_term->score + second_term->score;
                    cur->leftRight = {first_term, second_term};
                    cur->diagPrev = nullptr;
                }
            }
        }
    }

    string res = recover(dp[0][s.length() - 1]);
    cout << res;
}

int main() {
    cin >> s;
    pairBases();
}
