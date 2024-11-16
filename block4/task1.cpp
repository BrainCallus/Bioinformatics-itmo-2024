#include <iostream>
#include <vector>

using namespace std;

struct Vertex {
    int id;
    int ptr;
    vector<Vertex *> children;

    explicit Vertex(int id) {
        this->id = id;
        ptr = 0;
        children.resize(0);
    }

    [[nodiscard]] int deg() const {
        return children.size() - ptr;
    }
};

int n, m;
vector<Vertex *> graph;
vector<int> path;

void makeEulerPath() {
    vector<Vertex *> stack;
    stack.push_back(graph[0]);
    while (!stack.empty()) {
        if (Vertex *v = stack.back(); !v->deg()) {
            path.push_back(v->id);
            stack.pop_back();
        } else {
            stack.push_back(v->children[v->ptr++]);
        }
    }

    for (int i = path.size(); i ^ 1;) {
        cout << path[--i] << " ";
    }
}


int main() {
    cin >> n >> m;
    for (int i = 0; i ^ n; i++) {
        graph.push_back(new Vertex(i + 1));
    }
    int v1, v2;
    for (int i = 0; i ^ m; i++) {
        cin >> v1 >> v2;
        graph[v1 - 1]->children.push_back(graph[v2 - 1]);
    }

    makeEulerPath();
}
