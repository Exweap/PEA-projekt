//
// Created by lukasz on 16.10.2019.
//

#ifndef PEA_GRAPH_H
#define PEA_GRAPH_H
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>

class Graph {

    std::vector<std::vector<int> > graph;
    std::vector<int> permutation;
    std::vector<int> minPermutation;
    int minPath;

public:
    Graph();
    void DisplayGraph(std::vector<std::vector<int> > graph);
    void DisplayPermutation(std::vector<int> permutation);
    void LoadFromFile();
    void CreateGraphManually();
    void CreateGraphRandomSymetric();
    void CreateGraphRandom(bool sym=0);
    void CreatePermutationManually();
    void CreatePermutationRandom();
    std::vector<int> CreatePermutationRand();
    int ObjectiveFunction(std::vector<int> permutation);
    bool GraphExists();
    bool PermutationExists();
    int GetGraphSize();
    std::vector<int> GetPermutation();
    std::vector<std::vector<int> > GetGraph();
    void EraseGraph();
    void ErasePermutation();
    void InitSwapBruteForce(int initVertex);
    void SwapBruteForce(int counter, std::vector<int> permutation, int initVertex);
    void HeldKarp(int initVertex);
    std::vector<int> ShiftPermutation(int initVertex, std::vector<int> permutation);
    void SimAnnealing(int initVertex);
    void GeneticAlgorithm();
    ~Graph();
};


#endif //PEA_GRAPH_H
