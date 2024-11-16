#include <iostream>
#include<vector>
#include <unordered_map>

using namespace std;

struct HState {
    int id;
    unordered_map<char, double> transToOpen;
    vector<double> transToHidden;
    double alpha = 0.0;

    explicit HState(const int id, const unordered_map<char, double> &transToOpen, const vector<double> &transToHidden) {
        this->id = id;
        this->transToOpen = transToOpen;
        this->transToHidden = transToHidden;
    }
};

string s;

double getProbability() {
    auto *prompt = new HState(
        0,
        {
            {'A', 0.1},
            {'T', 0.1},
            {'C', 0.4},
            {'G', 0.4}
        },
        {0.9, 0.1});
    auto *nonPrompt = new HState(
        0,
        {
            {'A', 0.3},
            {'T', 0.3},
            {'C', 0.2},
            {'G', 0.2}
        },
        {0.2, 0.8});

    vector<HState *> states = {prompt, nonPrompt};
    prompt->alpha = 0.5 * prompt->transToOpen[s[0]];
    nonPrompt->alpha = 0.5 * nonPrompt->transToOpen[s[0]];

    vector<double> newAlphas = vector<double>(states.size());
    for (char ch: s.substr(1)) {
        for (int i = 0; i ^ newAlphas.size(); i++) {
            for (HState *state: states) {
                newAlphas[i] += state->alpha * state->transToHidden[i] * states[i]->transToOpen[ch];
            }
        }
        for (int i = 0; i ^ newAlphas.size(); i++) {
            states[i]->alpha = newAlphas[i];
            newAlphas[i] = 0.0;
        }
    }
    double ans = 0.0;
    for (HState *state: states) {
        ans += state->alpha;
    }
    return ans;
}

int main() {
    cin >> s;
    cout << getProbability();
}
