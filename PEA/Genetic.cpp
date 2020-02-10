#include "Graph.h"
#include "GeneticManager.h"
#include <cmath>
#include <algorithm>
#include <map>

void Graph::GeneticAlgorithm() {
    GeneticManager manager;
    std::vector<std::vector<int> > population, initPopulation;

    //generacja populacji początkowej
    int bestRouteInGen = INT_MAX, worstRouteInGen = INT_MIN;
    for(int i=0; i<GeneticManager::getPopulationSize(); i++) {
        std::vector<int> individual;
        individual = CreatePermutationRand();
        population.push_back(individual);

        if(bestRouteInGen > ObjectiveFunction(individual)) {
            bestRouteInGen = ObjectiveFunction(individual);
            this->minPath = bestRouteInGen;
            this->minPermutation = individual;
        }

        if(worstRouteInGen < ObjectiveFunction(individual))
            worstRouteInGen = ObjectiveFunction(individual);
    }
    initPopulation = population;
    int sumTime = 0, numOfTests = 10, sumMin = 0, testCounter = 1;
    int ochujchodzi=0;
    while(testCounter <= numOfTests) {

        //główna pętla
        auto start = std::chrono::steady_clock::now();
        population = initPopulation;
        int genCount = 0;
        while(genCount < GeneticManager::getGenerationNum()) {
            //std::cout <<++ochujchodzi <<std::endl;

            //sortowanie populacji względem przystosowania osobników (najlepsi na początku)
            for(int i=0; i<GeneticManager::getPopulationSize()-1; i++) {
                for(int j=1; j<GeneticManager::getPopulationSize()-i; j++) {
                    if(ObjectiveFunction(population[j-1]) > ObjectiveFunction(population[j]))
                        std::swap(population[j-1], population[j]);
                }
            }

            //przypisanie wartości przystosowania do poszczególnych osobników
            std::vector<double> fitnessArray;
            worstRouteInGen = ObjectiveFunction(population[GeneticManager::getPopulationSize()-1]);
            bestRouteInGen = ObjectiveFunction(population[0]);
            int solutionSpace = worstRouteInGen - bestRouteInGen;
            for(int i=0; i<GeneticManager::getPopulationSize(); i++) {
                int pathCost = ObjectiveFunction(population[i]);
                if(solutionSpace == 0)
                    fitnessArray.push_back(1);
                else
                    fitnessArray.push_back(fabs(((double)pathCost-(double)bestRouteInGen-(double)solutionSpace)) / ((double)solutionSpace));
            }

            //1. selekcja osobników do krzyżowania
            std::vector<std::vector<int> > selectedIndividuals;
            switch(GeneticManager::getSelectionType()) {

                //własny typ selekcji
                case 1: {
                    for(int i=0; i<GeneticManager::getPopulationSize(); i++) {
                        std::vector<int> individual = population[i];
                        if((double)rand()/(double)RAND_MAX <= fitnessArray[i]) {
                            selectedIndividuals.push_back(individual);
                        }
                    }
                }
                    break;

                //ruletka
                case 2: {
                    double probabilitySum = 0.0;
                    for(double i : fitnessArray)
                        probabilitySum+=i;

                    int numOfSelectedIndividuals = GeneticManager::getPopulationSize()/2;
                    std::vector<std::vector<int> > copyOfPopulation = population;
                    std::vector<double> copyOfFitnessArray = fitnessArray;
                    for(int i=0; i<numOfSelectedIndividuals; i++) {
                        double randDouble = (double)rand()/(double)RAND_MAX * probabilitySum;
                        double checkedProbability = 0.0;
                        for(int j=0; j<copyOfPopulation.size(); j++) {
                            checkedProbability += copyOfFitnessArray[j];
                            if(randDouble <= checkedProbability) {
                                probabilitySum -= copyOfFitnessArray[j];
                                selectedIndividuals.push_back(copyOfPopulation[j]);
                                copyOfPopulation.erase(copyOfPopulation.begin()+j);
                                copyOfFitnessArray.erase(copyOfFitnessArray.begin()+j);
                                break;
                            }
                        }
                    }
                }
                    break;

                //ranking
                case 3: {
                    for(int i=0; i<GeneticManager::getPopulationSize()/2; i++) {
                        selectedIndividuals.push_back(population[i]);
                    }
                }
                    break;
            }
            //2. Faza krzyżowania
            std::vector<std::vector<int> > offspring;
            if(selectedIndividuals.size()%2 == 1)
                selectedIndividuals.pop_back();
            for(int i=0; i<selectedIndividuals.size(); i+=2) {

                //losowanie rodziców
                std::vector<int> parent1, parent2;
                int drawn1 = rand()%selectedIndividuals.size(), drawn2;
                parent1 = selectedIndividuals[drawn1];
                selectedIndividuals.erase(selectedIndividuals.begin()+drawn1);
                drawn1 = rand()%selectedIndividuals.size();
                parent2 = selectedIndividuals[drawn1];
                selectedIndividuals.erase(selectedIndividuals.begin()+drawn1);

                //losowanie części chromosomu, która zostanie skopiowana
                drawn1 = rand()%((GetGraphSize()/2)-2);
                drawn2 = drawn1 + 1 + rand()%GetGraphSize()/2;

                //kopiowanie niezmiennych części chromosomów
                std::vector<int> child1, child2;
                child1.reserve(GetGraphSize());
                child2.reserve(GetGraphSize());
                child1.assign(GetGraphSize(), -1);
                child2.assign(GetGraphSize(), -1);
                for(int j=drawn1; j<=drawn2; j++) {
                    child1.erase(child1.begin()+j);
                    child1.insert(child1.begin()+j, parent2[j]);
                    child2.erase(child2.begin()+j);
                    child2.insert(child2.begin()+j, parent1[j]);
                }

                switch(GeneticManager::getCrossoverType()) {
                    //PMX
                    case 1: {
                        //mapowanie wierzchołków
                        std::map <int, int> vertexMapping1, vertexMapping2;
                        for(int j=0; j<GetGraphSize(); j++) {
                            vertexMapping1[parent1[j]] = parent2[j];
                            vertexMapping2[parent2[j]] = parent1[j];
                        }

                        //przypisanie reszty genów
                        for(int j=0; j<GetGraphSize(); j++) {
                            int gene1 = parent1[j], gene2 = parent2[j];

                            //przypisanie wierzcholków bezkonfliktowych dla pierwszego potomka
                            if(std::find(child1.begin(), child1.end(), gene1) == child1.end() && (j<drawn1 || j>drawn2)) {
                                child1.erase(child1.begin()+j);
                                child1.insert(child1.begin()+j, gene1);
                            }
                            //przypisanie wierzcholków konfliktowych dla pierwszego potomka
                            else {
                                bool finished = false;
                                while(!finished) {
                                    if(j>= drawn1 && j<=drawn2)
                                        break;
                                    gene1 = vertexMapping1[gene1];
                                    if(std::find(child1.begin(), child1.end(), gene1) == child1.end() && (j<drawn1 || j>drawn2)) {
                                        child1.erase(child1.begin()+j);
                                        child1.insert(child1.begin()+j, gene1);
                                        finished = true;
                                    }
                                }
                            }

                            //przypisanie wierzcholków bezkonfliktowych dla drugiego potomka
                            if(std::find(child2.begin(), child2.end(), gene2) == child2.end() && (j<drawn1 || j>drawn2)) {
                                child2.erase(child2.begin()+j);
                                child2.insert(child2.begin() + j, gene2);
                            }
                                //przypisanie wierzcholków konfliktowych dla drugiego potomka
                            else {
                                bool finished = false;
                                while(!finished) {
                                    if(j>= drawn1 && j<=drawn2)
                                        break;
                                    gene2 = vertexMapping2[gene2];
                                    if(std::find(child2.begin(), child2.end(), gene2) == child2.end() && (j<drawn1 || j>drawn2)) {
                                        child2.erase(child2.begin()+j);
                                        child2.insert(child2.begin()+j, gene2);
                                        finished = true;
                                    }
                                }
                            }
                        }
                    }
                        break;

                    //OX
                    case 2: {
                        //stworzenie sekwencji genów
                        std::vector<int> sequence1 = parent1, sequence2 = parent2;
                        std::rotate(sequence1.begin(), sequence1.begin()+drawn2+1, sequence1.end());
                        std::rotate(sequence2.begin(), sequence2.begin()+drawn2+1, sequence2.end());

                        //usunięcie powtórzonych genów z sekwencji
                        for(int j=0; j<sequence1.size(); j++) {
                            if (std::find(child1.begin(), child1.end(), sequence1[j]) != child1.end())
                                sequence1.erase(sequence1.begin() + j--);
                        }
                        for(int j=0; j<sequence2.size(); j++) {
                            if(std::find(child2.begin(), child2.end(), sequence2[j]) != child2.end())
                                sequence2.erase(sequence2.begin()+j--);
                        }

                        //uzupełnienie brakujących genów potomków
                        int marker = drawn2+1;
                        while(marker!=drawn1) {
                            if(marker == GetGraphSize()) {
                                marker = 0;
                                continue;
                            }
                            child1.erase(child1.begin()+marker);
                            child1.insert(child1.begin()+marker, sequence1[0]);
                            sequence1.erase(sequence1.begin()+0);
                            child2.erase(child2.begin()+marker);
                            child2.insert(child2.begin()+marker, sequence2[0]);
                            sequence2.erase(sequence2.begin()+0);
                            marker++;
                        }
                    }
                        break;
                }
                offspring.push_back(child1);
                offspring.push_back(child2);
            }

            //3. Faza mutacji
            double mutationChance;
            for(int i=0; i<offspring.size(); i++) {
                mutationChance = (double)(rand()+1)/(double)(RAND_MAX+1);
                if(mutationChance <= GeneticManager::getMutationProbability()) {
                    switch (GeneticManager::getMutationType()) {
                        //mutacja swap
                        case 1: {
                            int drawn1 = rand() % GetGraphSize(), drawn2 = rand() % GetGraphSize();
                            while (drawn2 == drawn1)
                                drawn2 = rand() % GetGraphSize();
                            std::swap(offspring[i][drawn1], offspring[i][drawn2]);
                        }
                            break;
                        //mutacja inverse
                        case 2: {
                            int drawn1 = rand() % GetGraphSize(), drawn2 = rand() % GetGraphSize();
                            while (drawn2 == drawn1)
                                drawn2 = rand() % GetGraphSize();
                            if (drawn1 < drawn2)
                                std::reverse(offspring[i].begin()+drawn1, offspring[i].begin()+drawn2);
                            else
                                std::reverse(offspring[i].begin()+drawn2, offspring[i].begin()+drawn1);
                        }
                            break;
                    }
                }
            }

            //4. Faza oceny potomków
            for(int i=0; i<offspring.size(); i++) {
                int worstAncestor = -1, worstAncestorValue = INT_MIN;
                for(int j=0; j<GeneticManager::getPopulationSize(); j++) {
                    if(ObjectiveFunction(population[j]) > worstAncestorValue) {
                        worstAncestorValue = ObjectiveFunction(population[j]);
                        worstAncestor = j;
                        worstRouteInGen = worstAncestorValue;
                    }
                }
                if(ObjectiveFunction(offspring[i]) < worstAncestorValue) {
                    population.erase(population.begin()+worstAncestor);
                    population.insert(population.begin()+worstAncestor, offspring[i]);
                    if(ObjectiveFunction(offspring[i]) < this->minPath) {
                        this->minPath = ObjectiveFunction(offspring[i]);
                        this->minPermutation = offspring[i];
                        bestRouteInGen = this->minPath;
                    }
                }
            }
            genCount++;
        }

        auto end = std::chrono::steady_clock::now();
        std::cout << "Nr testu: " << testCounter << "\n";
        std::cout << "Minimalna odnaleziona dlugosc sciezki wynosi: " << this->minPath << "\n";
        std::cout << "dla permutacji ";
        DisplayPermutation(this->minPermutation);
        std::cout << "Czas operacji wyniosl "<< std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()<< " ms"<< "\n";
        sumTime += std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        std::cout << "Sredni czas ze wszystkich testow wyniosl "<< (double)sumTime/(double)testCounter << " ms"<< "\n";
        sumMin += this->minPath;
        std::cout << "Sredni dlugosc sciezki ze wszystkich testow wynosi: "<< (double)sumMin/(double)testCounter << "\n\n";
        this->minPermutation.erase(this->minPermutation.begin(), this->minPermutation.end());
        this->minPath = INT_MAX;
        testCounter++;
    }
}