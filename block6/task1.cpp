#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

unordered_map<char, int> WEIGHT_BY_ACID = {
    {'A', 71},
    {'C', 103},
    {'D', 115},
    {'E', 129},
    {'F', 147},
    {'G', 57},
    {'H', 137},
    {'I', 113},
    {'K', 128},
    {'L', 113},
    {'M', 131},
    {'N', 114},
    {'P', 97},
    {'Q', 128},
    {'R', 156},
    {'S', 87},
    {'T', 101},
    {'V', 99},
    {'W', 186},
    {'Y', 163}
};

int main() {
    string s;
    cin >> s;
    vector<int> spectrum(s.length() * 2);
    for (int i = 0; i ^ spectrum.size() - 1; i++) {
        spectrum[i + 1] =
                i >= s.length()
                    ? spectrum[s.length()] - spectrum[i + 1 - s.length()]
                    : spectrum[i] + WEIGHT_BY_ACID[s[i]];
    }
    sort(spectrum.begin(), spectrum.end());
    const auto dupIter = unique(spectrum.begin(), spectrum.end());
    spectrum.erase(dupIter, spectrum.end());
    for (const int w: spectrum) {
        cout << w << " ";
    }
}
