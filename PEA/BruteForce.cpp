//
// Created by lukasz on 13.11.2019.
//
#include "Graph.h"

void Graph::InitSwapBruteForce(int initVertex) {
    //ErasePermutation();
    auto start = std::chrono::steady_clock::now();
    for(int i=0; i<this->graph.size(); i++) {
        if(initVertex == i)
            continue;
        this->permutation.push_back(i);
    }
    SwapBruteForce(this->permutation.size(), this->permutation, initVertex);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Minimalna dlugosc sciezki wynosi " << this->minPath << "\n";
    std::cout << "dla permutacji ";
    DisplayPermutation(this->minPermutation);
    std::cout << "Czas operacji wyniosl "<< std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()<< " ms"<< "\n";
    this->minPermutation.erase(this->minPermutation.begin(), this->minPermutation.end());
    this->minPath = INT_MAX;
}

void Graph::SwapBruteForce(int k, std::vector<int> permutation, int initVertex) {
    if(k == 1) {
        permutation.insert(permutation.begin(), initVertex);
        int obj = ObjectiveFunction(permutation);
        if(this->minPath == INT_MAX) {
            this->minPath = obj;
            this->minPermutation = permutation;
        }
        else if(this->minPath > obj) {
            this->minPath = obj;
            this->minPermutation = permutation;
        }
        return;
    }
    else
        for(int i=0; i<k; i++) {
            std::swap(permutation[i], permutation[k-1]);
            SwapBruteForce(k-1, permutation, initVertex);
            std::swap(permutation[i], permutation[k-1]);
        }
}
