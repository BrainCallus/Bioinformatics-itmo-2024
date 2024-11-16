#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <algorithm>
#include <cmath>

using namespace std;

struct HState {
    int id;
    double alpha = 0;
    double beta = 0;
    unordered_map<char, double> transToOpen;
    vector<double> transToHidden;

    explicit HState(const int id, const unordered_map<char, double> &transToOpen, const vector<double> &transToHidden) {
        this->id = id;
        this->transToOpen = transToOpen;
        this->transToHidden = transToHidden;
    }
};

string s;

double forward(vector<vector<HState *> > &stateTable) {
    for (char ch: s.substr(1)) {
        vector<HState *> newLine;
        for (HState *state_i: stateTable.back()) {
            auto *newState = new HState(state_i->id, state_i->transToOpen, state_i->transToHidden);
            for (HState *state_j: stateTable.back()) {
                newState->alpha += state_j->alpha * state_j->transToHidden[state_i->id] * state_i->transToOpen[ch];
            }
            newLine.push_back(newState);
        }
        stateTable.push_back(newLine);
    }
    double p_0 = 0.0;
    for (const HState *state: stateTable.back()) {
        p_0 += state->alpha;
    }
    return p_0;
}

vector<vector<double> > backward(double p_0, vector<vector<HState *> > &stateTable) {
    vector<vector<double> > probs;
    for (int i = stateTable.size() - 1; i >= 0; i--) {
        vector<double> cur_probs;
        for (HState *state: stateTable[i]) {
            if (i == stateTable.size() - 1) {
                state->beta = 1;
            } else {
                for (HState *state_j: stateTable[i + 1]) {
                    state->beta += state_j->beta * state_j->transToOpen[s[i + 1]] * state->transToHidden[state_j->id];
                }
            }
            cur_probs.push_back(state->alpha * state->beta / p_0);
        }
        probs.push_back(cur_probs);
    }
    reverse(probs.begin(), probs.end());
    return probs;
}

void getProbability() {
    unordered_map<char, double> promptOpen = {
        {'A', 0.1},
        {'T', 0.1},
        {'C', 0.4},
        {'G', 0.4}
    };
    unordered_map<char, double> nonPromptOpen = {
        {'A', 0.3},
        {'T', 0.3},
        {'C', 0.2},
        {'G', 0.2}
    };
    const vector<double> promptHidden = {0.9, 0.1};
    const vector<double> nonPromptHidden = {0.2, 0.8};
    auto *initPrompt = new HState(
        0,
        promptOpen,
        promptHidden);
    auto *initNonPrompt = new HState(
        1,
        nonPromptOpen,
        nonPromptHidden);

    vector<vector<HState *> > stateTable;
    stateTable.push_back({initPrompt, initNonPrompt});
    initPrompt->alpha = 0.5 * initPrompt->transToOpen[s[0]];
    initNonPrompt->alpha = 0.5 * initNonPrompt->transToOpen[s[0]];

    vector<vector<double> > probs = backward(forward(stateTable), stateTable);

    for (int i = 0; i ^ stateTable.back().size(); i++) {
        for (vector<double> row: probs) {
            cout << round(row[i] / 0.01) * 0.01 << " ";
        }
        cout << "\n";
    }
}

int main() {
    cin >> s;
    getProbability();
}
