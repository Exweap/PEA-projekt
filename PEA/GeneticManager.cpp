//
// Created by exwea on 19.01.2020.
//

#include "GeneticManager.h"
#include "Menu.h"
#include "Graph.h"
#include <iostream>

int GeneticManager::populationSize = 100;
int GeneticManager::generationNum = 10;
int GeneticManager::selectionType = 1;
int GeneticManager::mutationType = 1;
int GeneticManager::crossoverType = 1;
double GeneticManager::mutationProbability = 0.1;

GeneticManager::GeneticManager() {
    Menu::ClearScreen();
    std::cout << "Rozmiar populacji: ";
    std::cin >> GeneticManager::populationSize;
    std::cout << "Liczba generacji: ";
    std::cin >> GeneticManager::generationNum;
    std::cout << "Prawdopodobienstwo mutacji: ";
    std::cin >> GeneticManager::mutationProbability;
    std::cout << "Typ selekcji: (1 - probabilistyczna, 2 - ruletka, 3 - ranking)";
    std::cin >> GeneticManager::selectionType;
    std::cout << "Typ krzyzowania: (1 - PMX, 2 - OX)";
    std::cin >> GeneticManager::crossoverType;
    std::cout << "Typ mutacji: (1 - swap, 2 - inverse)";
    std::cin >> GeneticManager::mutationType;
}

int GeneticManager::getPopulationSize() {
    return populationSize;
}

int GeneticManager::getGenerationNum() {
    return generationNum;
}

int GeneticManager::getSelectionType() {
    return selectionType;
}

int GeneticManager::getMutationType() {
    return mutationType;
}

int GeneticManager::getCrossoverType() {
    return crossoverType;
}

double GeneticManager::getMutationProbability() {
    return mutationProbability;
}