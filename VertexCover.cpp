#include <omp.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

using edge = pair<int, int>;

bool isVertexCover(vector<int> V, vector<edge> E) {
    set<int> V_set(V.begin(), V.end());
    for (auto e : E) {
        if (V_set.find(e.first) == V_set.end() && V_set.find(e.second) == V_set.end()) {
            return false;
        }
    }
    return true;
}

uint64_t total_nodes = 0;
vector<edge> E;
int v, e, a, b;
int minimum_sol = INT8_MAX;
vector<int> vector_sol;

void printVector(vector<int> V) {
    cout << "[ ";
    for (int i = 0; i < V.size(); i++) {
        cout << V[i] << ",";
    }
    cout << "]";
}

void search(vector<int> V, int lastNode) {
    total_nodes += 1;
    int s = V.size();
    bool ret = false;

    string ans;
    if (isVertexCover(V, E)) {
        ans = " TRUE";
#pragma omp critical
        {
            if (s < minimum_sol) {
                minimum_sol = s;
                vector_sol = V;
                printVector(V);
                cout << ans;
                cout << " current total nodes: " << total_nodes << endl;
            }
            ret = true;
        }
        if (ret) return;
    } else {
        // ans = " FALSE";
        // printVector(V);
        // cout << minimum_sol << " " << ans << endl;
    }

    for (int i = lastNode + 1; i < v; i++) {
#pragma omp task untied
        {
            // #pragma omp critical
            //             {
            if (s + 1 >= minimum_sol) {
                ret = true;

            } else {
                vector<int> tmp(V.begin(), V.end());
                tmp.push_back(i);
                search(tmp, i);
            }
            // }
        }
        if (ret) return;
    }
}
int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    string outputFile;
    string file_name;
    // string file_name = "grid-20-31";
    // string file_name = "grid-25-40";
    // string file_name = "grid-30-49";
    // string file_name = "tree-40-39";
    if (argc == 3) {
        outputFile = argv[2];
        file_name = argv[1];
        cout << "<<Command is correct and the processes will be started>>\n";
    } else {
        cout << "Please insert arguments in format: " << argv[0] << " <input> <output>\n";
        return 1;
    }
    FILE* in = fopen(file_name.c_str(), "r");
    ifstream input;
    input.open(file_name);
    if (input.is_open()) {
        input >> v >> e;
        for (int i = 0; i < e; i++) {
            input >> a >> b;
            E.push_back(edge(a, b));
        }

        input.close();
    }
    omp_set_num_threads(omp_get_max_threads() * 6);

#pragma omp parallel for

    for (int i = 0; i < v; i++) {
        vector<int> V;
        V.push_back(i);
        search(V, i);
    }

    cout << "-------------------------------------------------------\n";
    cout << "Total visited nodes: " << total_nodes << endl;
    cout << "Minimum solutions: " << minimum_sol << endl;
    printVector(vector_sol);

    int ans[v];
    memset(ans, 0, v * sizeof(ans[0]));
    for (int i = 0; i < vector_sol.size(); i++) {
        ans[vector_sol[i]] = 1;
    }

    string final_ans;
    for (int i = 0; i < v; i++) {
        final_ans += to_string(ans[i]);
    }
    cout << ": " << final_ans << endl;
    ofstream output;
    output.open(outputFile);
    output << final_ans;
    output.close();
    return 0;
}