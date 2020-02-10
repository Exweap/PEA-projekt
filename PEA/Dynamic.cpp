//
// Created by lukasz on 13.11.2019.
//

#include <iostream>
#include <algorithm>
#include "Graph.h"

std::vector<int> Graph::ShiftPermutation(int initVertex, std::vector<int> permutation) {
    for(int i=0; i<GetGraphSize(); i++) {
        if(initVertex == permutation[0])
            return permutation;
        else {
            std::rotate(permutation.begin(), permutation.begin()+1, permutation.end());
        }
    }
    return permutation;
}

void Graph::HeldKarp(int initVertex) {
    //ErasePermutation();
    auto start = std::chrono::steady_clock::now();
    //Dwuwymiarowy vector best jest dynamicznie uzupełniany o informacje o najkrótszej drodze
    std::vector< std::vector<int> > best( 1<<(GetGraphSize()-1), std::vector<int>( GetGraphSize(), INT_MAX ) );
    this->minPath = INT_MAX;
    for (int visited = 1; visited < (1<<(GetGraphSize()-1)); ++visited) {
        for (int last = 0; last < (GetGraphSize() - 1); ++last) {

            if (!(visited & 1 << last))
                continue;
            if(visited == 1 << last)
                best[visited][last] = this->graph[GetGraphSize()-1][last];
            else {
                int preVisited = visited ^ 1<<last;
                for(int previous = 0; previous < GetGraphSize()-1; ++previous) {
                    if (!(preVisited & 1<<previous))
                        continue;
                    best[visited][last] = std::min(best[visited][last], this->graph[previous][last] + best[preVisited][previous]);
                }
            }
        }
    }

    for(int last = 0; last < GetGraphSize()-1; ++last) {
        this->minPath = std::min(minPath, this->graph[last][GetGraphSize()-1] + best[(1<<(GetGraphSize()-1))-1][last]);
    }

    std::vector<int> minPerm;
    int minTemp = this->minPath;
    int lastVisited = GetGraphSize()-1;
    int visited = (1<<(GetGraphSize()-1))-1;
    while(visited !=0) {
        for(int j=0; j<GetGraphSize()-1; j++) {
            if(minTemp - this->graph[j][lastVisited] == best[visited][j]) {
                minTemp -= this->graph[j][lastVisited];
                visited -= 1 << j;
                lastVisited = j;
                this->minPermutation.push_back(j);
            }
        }
    }
    this->minPermutation.push_back(GetGraphSize()-1);
    std::reverse(this->minPermutation.begin(), this->minPermutation.begin()+this->minPermutation.size());
    auto end = std::chrono::steady_clock::now();

    this->minPermutation = ShiftPermutation(initVertex, this->minPermutation);
    std::cout << "Minimalna dlugosc sciezki wynosi " << this->minPath << "\n";
    std::cout << "dla permutacji ";
    DisplayPermutation(this->minPermutation);
    std::cout << "Czas operacji wyniosl "<< std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()<< " ms"<< "\n";
    this->minPermutation.erase(this->minPermutation.begin(), this->minPermutation.end());
    this->minPath = INT_MAX;
}