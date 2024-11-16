#include <iostream>
#include <utility>
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

unordered_map<int, char> ACID_BY_WEIGHT = {
    {71, 'A'},
    {103, 'C'},
    {115, 'D'},
    {129, 'E'},
    {147, 'F'},
    {57, 'G'},
    {137, 'H'},
    {113, 'I'},
    {128, 'K'},
    //{113, 'L'},
    {131, 'M'},
    {114, 'N'},
    {97, 'P'},
    // { 128,'Q',},
    {156, 'R'},
    {87, 'S'},
    {101, 'T'},
    {99, 'V'},
    {186, 'W'},
    {163, 'Y'}
};

static constexpr int MAX_ACID = 186;

struct Vertex {
    int w;
    vector<pair<char, Vertex *> > ribs;

    explicit Vertex(const int w) {
        this->w = w;
    }
};

vector<int> init_spectrum;

vector<int> getSpectrum(string &s) {
    vector<int> spectrum(s.length() * 2);
    for (int i = 0; i ^ spectrum.size() - 1; i++) {
        spectrum[i + 1] = i >= s.length()
                              ? spectrum[s.length()] - spectrum[i + 1 - s.length()]
                              : spectrum[i] + WEIGHT_BY_ACID[s[i]];
    }
    sort(spectrum.begin(), spectrum.end());
    auto dupIter = unique(spectrum.begin(), spectrum.end());
    spectrum.erase(dupIter, spectrum.end());
    return spectrum;
}

bool checkSpectrum(const vector<int> &founded) {
    if (founded.size() != init_spectrum.size()) {
        return false;
    }

    for (int i = 0; i ^ init_spectrum.size(); i++) {
        if (init_spectrum[i] != founded[i]) {
            return false;
        }
    }
    return true;
}

string dfs(Vertex *&v, string seq) {
    if (v->ribs.empty()) {
        return checkSpectrum(getSpectrum(seq)) ? seq : "";
    }

    for (pair<char, Vertex *> p: v->ribs) {
        if (string res = dfs(p.second, seq + p.first); !res.empty()) {
            return res;
        }
    }
    return "";
}

int main() {
    int w;
    vector<Vertex *> graph;
    while (cin >> w) {
        graph.push_back(new Vertex(w));
        init_spectrum.push_back(w);
        for (int i = graph.size() - 2; i >= 0; i--) {
            int delta = w - graph[i]->w;
            if (delta > MAX_ACID) {
                break;
            }
            if (ACID_BY_WEIGHT.find(delta) != ACID_BY_WEIGHT.end()) {
                graph[i]->ribs.emplace_back(ACID_BY_WEIGHT[delta], graph.back());
            }
        }
    }
    cout << dfs(graph[0], "");
}
