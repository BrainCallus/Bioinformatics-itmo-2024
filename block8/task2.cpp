#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Node {
    int id;
    char nameOption;
    float distToBottom;
    int level;
    int cluster_size;
    Node *ancestor;
    float distToAncestor;
    unordered_map<int, pair<float, Node *> > distances;
    vector<Node *> children;

    explicit Node(int id) {
        this->id = id;
        this->nameOption = 0;
        this->ancestor = nullptr;
        this->distToAncestor = -1;
        this->distToBottom = 0;
        this->level = 0;
        this->cluster_size = 1;
    }

    explicit Node(int id, char nameOption) {
        this->id = id;
        this->nameOption = nameOption;
        this->ancestor = nullptr;
        this->distToAncestor = -1;
        this->distToBottom = 0;
        this->level = 0;
        this->cluster_size = 1;
    }

    explicit Node(int id, int cluster_size) {
        this->id = id;
        this->nameOption = 0;
        this->ancestor = nullptr;
        this->distToAncestor = -1;
        this->distToBottom = 0;
        this->level = 0;
        this->cluster_size = cluster_size;
    }

    void addChild(Node *child, float commonDist) {
        children.push_back(child);
        child->distToAncestor = commonDist - child->distToBottom;
        child->ancestor = this;
        if (child->level >= this->level) {
            this->level = child->level + 1;
        }
    }

    bool isTerm() const {
        return nameOption > 64;
    }
};

int n;
int MAX_ID;
unordered_map<int, Node *> distTable;

bool cmpForPrint(Node *&a, Node *&b) {
    if (a->isTerm() && b->isTerm()) {
        return a->nameOption < b->nameOption;
    }

    return a->level < b->level;
}

pair<pair<int, int>, float> getArgMin() {
    int i, j;
    float min_dist = 1e10;
    for (pair<int, Node *> entry: distTable) {
        for (pair<int, pair<float, Node *> > dst_entry: entry.second->distances) {
            if (dst_entry.second.first < min_dist) {
                min_dist = dst_entry.second.first;
                i = entry.second->id;
                j = dst_entry.second.second->id;
            }
        }
    }
    return {{i, j}, min_dist};
}

float calcWeightedDist(pair<float, Node *> entryA, pair<float, Node *> entryB) {
    return (entryA.first * (float) entryA.second->cluster_size + entryB.first * (float) entryB.second->cluster_size) /
           (float) (entryA.second->cluster_size + entryB.second->cluster_size);
}

void recalcDistancesForMerged(Node *&newEntry, int &firstIdx, int &secondIdx, float sizeA, float sizeB) {
    for (pair<int, Node *> entry: distTable) {
        if (entry.first != firstIdx && entry.first != secondIdx) {
            float nd =
                    (entry.second->distances[firstIdx].first * sizeA + entry.second->distances[secondIdx].first * sizeB)
                    / (newEntry->cluster_size);
            calcWeightedDist(entry.second->distances[firstIdx], entry.second->distances[secondIdx]);
            float newDist = calcWeightedDist(entry.second->distances[firstIdx],
                                             entry.second->distances[secondIdx]);
            newEntry->distances[entry.first] = {newDist, entry.second};
            if (nd != newDist) {
                cout << "!!!\n";
            }
            entry.second->distances[MAX_ID] = {newDist, newEntry};
        }
    }
}

void buildTree() {
    while (distTable.size() > 1) {
        pair<pair<int, int>, float> argMin = getArgMin();
        int first_idx = argMin.first.first;
        int second_idx = argMin.first.second;
        Node *merged = new Node(MAX_ID, distTable[first_idx]->cluster_size + distTable[second_idx]->cluster_size);

        recalcDistancesForMerged(merged, first_idx, second_idx,
                                 (float) distTable[first_idx]->cluster_size,
                                 (float) distTable[second_idx]->cluster_size);
        distTable[MAX_ID] = merged;
        merged->addChild(distTable[first_idx], argMin.second / 2);
        merged->addChild(distTable[second_idx], argMin.second / 2);
        merged->distToBottom = argMin.second / 2;

        MAX_ID++;
        distTable.erase(first_idx);
        distTable.erase(second_idx);
        for (pair<int, Node *> entry: distTable) {
            entry.second->distances.erase(first_idx);
            entry.second->distances.erase(second_idx);
        }
    }
}

void printTreeNewick(Node *v) {
    if (!v->children.empty()) {
        cout << "(";
        sort(v->children.begin(), v->children.end(), cmpForPrint);
        for (int i = 0; i < v->children.size(); i++) {
            printTreeNewick(v->children[i]);
            if (i != v->children.size() - 1) {
                cout << ",";
            }
        }
        cout << ")";
    }

    if (v->ancestor) {
        if (v->isTerm()) {
            cout << v->nameOption;
        }
        cout << ":";
        printf("%.2f", v->distToAncestor);
    }
}

int main() {
    cin >> n;
    for (int i = 0; i ^ n; i++) {
        distTable[i] = new Node(i, (char) (i + 65));
    }
    MAX_ID = n;

    for (int i = 0; i ^ n; i++) {
        int d;
        for (int j = 0; j ^ n; j++) {
            cin >> d;
            if (j != i) {
                distTable[i]->distances[j] = {d, distTable[j]};
                distTable[j]->distances[i] = {d, distTable[i]};
            }
        }
    }

    buildTree();
    printTreeNewick(distTable[MAX_ID - 1]);
}
