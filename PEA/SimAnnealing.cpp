//
// Created by exwea on 09.12.2019.
//

#include "Graph.h"
#include "AnnealingManager.h"
#include <algorithm>

void Graph::SimAnnealing(int initVertex) {
    srand(time(NULL));
    AnnealingManager settings;
    std::vector<int> solution, initSolution;
    switch(settings.GetInitSolutionType()) {
        case 'l': {
            CreatePermutationRandom();
            solution = this->permutation;
            break;
        }
        case 'n': {
            for (int i = 0; i < this->GetGraphSize(); i++)
                solution.push_back(i);
            break;
        }
        case 'z': {
            auto startZ = std::chrono::steady_clock::now();
            solution.push_back(initVertex);
            int lastVisited = initVertex;
            int nextVisited = -1;
            for(int i = 0; i < this->GetGraphSize()-1; i++) {
                int minTemp = INT_MAX;
                for(int j = 0; j < this->GetGraphSize(); j++) {
                    if(std::find(solution.begin(), solution.end(), j) != solution.end()) {
                        continue;
                    }
                    if(this->graph[lastVisited][j] < minTemp) {
                        minTemp = this->graph[lastVisited][j];
                        nextVisited = j;
                    }
                }
                solution.push_back(nextVisited);
                lastVisited = nextVisited;
            }
            auto endZ = std::chrono::steady_clock::now();
            std::cout <<"Czas wykonania algorytmu zachłannego: " << std::chrono::duration_cast<std::chrono::milliseconds>(endZ-startZ).count() << std::endl;
            break;
        }
    }
    initSolution = solution;
    bool repeatTest = true;
    int sumTime = 0, numOfTests = 0, sumMin = 0;
    while(repeatTest) {
        solution = initSolution;
        std::cout <<"\nTest nr: " << ++numOfTests << "\n";
        std::cout <<"Poczatkowa sciezka: \n" ;
        DisplayPermutation(solution);
        std::cout <<"o wartosci: " << ObjectiveFunction(solution)<< "\n" ;
        this->minPermutation = solution;
        this->minPath = ObjectiveFunction(solution);
        int numOfIters = 0;
        auto start = std::chrono::steady_clock::now();
        while (settings.GetTemperature() > 1) {
            std::vector<int> newSolution = solution;
            int pos1 = rand()%this->GetGraphSize(), pos2 = rand()%this->GetGraphSize();
            while (pos1 == pos2) {
                pos2 = rand()%this->GetGraphSize();
            }
            std::iter_swap(newSolution.begin() + pos1, newSolution.begin() + pos2);

            int costOld = ObjectiveFunction(solution);
            int costNew = ObjectiveFunction(newSolution);
            if(settings.GetAcceptanceProbability(costOld, costNew) > (double)rand()/RAND_MAX)
                solution = newSolution;

            if(ObjectiveFunction(solution) < ObjectiveFunction(this->minPermutation)) {
                this->minPermutation = solution;
                this->minPath = ObjectiveFunction(this->minPermutation);
            }
            settings.Cool();
            numOfIters++;
        }
        auto end = std::chrono::steady_clock::now();
        std::cout << "Liczba iteracji " << numOfIters << "\n";
        std::cout << "Minimalna odnaleziona dlugosc sciezki wynosi: " << this->minPath << "\n";
        std::cout << "dla permutacji ";
        DisplayPermutation(this->minPermutation);
        std::cout << "Czas operacji wyniosl "<< std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()<< " ms"<< "\n";
        sumTime += std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        std::cout << "Sredni czas ze wszystkich testow wyniosl "<< (double)sumTime/(double)numOfTests << " ms"<< "\n";
        sumMin += this->minPath;
        std::cout << "Sredni dlugosc sciezki ze wszystkich testow wynosi: "<< (double)sumMin/(double)numOfTests << "\n";
        this->minPermutation.erase(this->minPermutation.begin(), this->minPermutation.end());
        this->minPath = INT_MAX;

        std::cout << "Czy powtorzyc test? (0 - nie, 1 - tak)";
        std::cin >> repeatTest;
        settings.Reset();
    }
}