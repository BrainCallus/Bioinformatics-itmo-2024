#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <map>

using namespace std;

struct Node {
    string seq;
    int id;
    int level;
    int cluster_size;
    char nameOption;
    float distToBottom;
    float distToAncestor;
    Node *ancestor;
    map<int, pair<float, Node *> > distances;
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
        child->distToAncestor = commonDist; // - child->distToBottom;
        child->ancestor = this;
        if (child->level >= this->level) {
            this->level = child->level + 1;
        }
    }

    [[nodiscard]] bool isTerm() const {
        return nameOption > 64;
    }

    [[nodiscard]] float getSumDists() const {
        float ans = 0;

        for (pair<int, pair<float, Node *> > d: distances) {
            ans += d.second.first;
        }
        return ans;
    }
};

struct ArgMin {
    Node *firstEntry;
    Node *secondEntry;
    float sums;
};

int n;
int MAX_ID;
map<int, Node *> distTable;

bool cmpForPrint(Node *&a, Node *&b) {
    if (a->isTerm() && b->isTerm()) {
        return a->nameOption < b->nameOption;
    }

    return a->level < b->level;
}

ArgMin findArgMin(int curN) {
    ArgMin res = {nullptr, nullptr, -1};
    float minVal = 1e10;
    for (pair<int, Node *> e: distTable) {
        Node *entry_i = e.second;
        float dists_i = entry_i->getSumDists();
        for (pair<int, pair<float, Node *> > neigh: entry_i->distances) {
            Node *entry_j = neigh.second.second;
            float dists_j = entry_j->getSumDists();
            float q_ij = ((float) curN - 2) * entry_i->distances[entry_j->id].first - dists_i - dists_j;
            if (q_ij < minVal) {
                minVal = q_ij;
                res.firstEntry = entry_i;
                res.secondEntry = entry_j;
                res.sums = dists_i - dists_j;
            }
        }
    }
    return res;
}

void recalcDistances(Node *&firstEntry, Node *secondEntry, Node *&newEntry) {
    float d_ij = firstEntry->distances[secondEntry->id].first;
    for (pair<int, Node *> entry: distTable) {
        if (entry.first != firstEntry->id && entry.first != secondEntry->id) {
            float newDist = (firstEntry->distances[entry.first].first + secondEntry->distances[entry.first].first -
                             d_ij) / 2;
            newEntry->distances[entry.first] = {newDist, entry.second};
            entry.second->distances[newEntry->id] = {newDist, newEntry};
        }
    }
}

void buildTree() {
    while (distTable.size() > 1) {
        if (distTable.size() == 2) {
            Node *root = new Node(MAX_ID);
            float d = 0;
            for (pair<int, pair<float, Node *> > ds: distTable[MAX_ID - 1]->distances) {
                d += ds.second.first;
            }
            for (pair<int, Node *> entry: distTable) {
                root->addChild(entry.second, d / 2);
            }
            MAX_ID++;
            distTable[root->id] = root;

            break;
        }

        int curN = distTable.size();
        ArgMin argMn = findArgMin(curN);

        auto newNode = new Node(MAX_ID);
        recalcDistances(argMn.firstEntry, argMn.secondEntry, newNode);
        distTable[newNode->id] = newNode;
        float dist1 = 0.5 * (argMn.firstEntry->distances[argMn.secondEntry->id].first + argMn.sums / (
                                 (float) curN - 2));
        newNode->addChild(argMn.firstEntry, dist1);
        newNode->addChild(argMn.secondEntry, argMn.firstEntry->distances[argMn.secondEntry->id].first - dist1);

        MAX_ID++;
        distTable.erase(argMn.firstEntry->id);
        distTable.erase(argMn.secondEntry->id);
        for (const pair<int, Node *> entry: distTable) {
            entry.second->distances.erase(argMn.firstEntry->id);
            entry.second->distances.erase(argMn.secondEntry->id);
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
        printf("%.1f", v->distToAncestor);
    }
}

float hammingDist(const string &s1, const string &s2) {
    float dist = 0;
    for (int i = 0; i ^ s1.length(); i++) {
        if (s1[i] != s2[i]) {
            dist++;
        }
    }
    return dist;
}

int main() {
    cin >> n;
    for (int i = 0; i ^ n; i++) {
        distTable[i] = new Node(i, (char) (i + 65));
    }
    MAX_ID = n;

    for (int i = 0; i ^ n; i++) {
        string s;
        cin >> s;
        distTable[i]->seq = s;
        for (int j = i - 1; j >= 0; j--) {
            float dist = hammingDist(s, distTable[j]->seq);
            distTable[i]->distances[j] = {dist, distTable[j]};
            distTable[j]->distances[i] = {dist, distTable[i]};
        }
    }

    buildTree();
    printTreeNewick(distTable[MAX_ID - 1]);
}
