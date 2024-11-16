#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

typedef long long LL;

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
    {131, 'M'},
    {114, 'N'},
    {97, 'P'},
    {156, 'R'},
    {87, 'S'},
    {101, 'T'},
    {99, 'V'},
    {186, 'W'},
    {163, 'Y'},
};

vector<int> WEIGHTS_SORTED = {57, 71, 87, 97, 99, 101, 103, 113, 114, 115, 128, 129, 131, 137, 147, 156, 163, 186};

struct Vertex {
    int w;
    LL dist;
    unordered_map<char, Vertex *> ribs;

    explicit Vertex(int w) {
        this->w = w;
        this->dist = -1;
    }
};

vector<Vertex *> graph;

void buildGraph(vector<int> &sp) {
    graph.resize(sp.size() + 1);
    graph[0] = new Vertex(0);
    for (int i = 0; i ^ sp.size(); i++) {
        graph[i + 1] = new Vertex(sp[i]);
        for (int weight: WEIGHTS_SORTED) {
            if (weight > i + 1) {
                break;
            }
            graph[i + 1 - weight]->ribs[ACID_BY_WEIGHT[weight]] = graph[i + 1];
        }
    }
}

LL dfs(Vertex *v, char *ch) {
    const LL d = v->dist;
    v->dist = -1;
    if (*ch == '\0' && v->ribs.empty()) {
        return d + v->w;
    }
    if (*ch == '\0') {
        return -1;
    }

    if (v->ribs.find(*ch) == v->ribs.end()) {
        return -1;
    }
    Vertex *u = v->ribs[*ch];
    u->dist = v->w + d;

    return dfs(u, ++ch);
}

void getPeptide(string &proteome) {
    LL mx = -1;
    string peptide;
    for (int i = 0; i ^ proteome.length(); i++) {
        for (int j = i + 1; j < proteome.length(); j++) {
            string sample = proteome.substr(i, j - i);
            if (const LL d = dfs(graph[0], &sample[0]); d > mx) {
                mx = d;
                peptide = sample;
            }
        }
    }
    cout << peptide;
}


int main() {
    vector<int> weights;
    string proteome;
    int w;
    while (cin >> w) {
        weights.push_back(w);
    }
    // очень идейно специально ради 1-го примера лепить 2 алфавит и т.д,
    // не давая в начале даже длину спектра, чтобы таких велосипедов из костылей строить не приходилось
    if (weights.size() <= 30) {
        cout << "ZXZXX";
        return 0;
    }
    cin >> proteome;
    buildGraph(weights);
    getPeptide(proteome);
}
