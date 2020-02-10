//
// Created by exwea on 09.12.2019.
//

#include "AnnealingManager.h"
#include "Menu.h"
#include <iostream>
#include <cmath>

AnnealingManager::AnnealingManager() {
    Menu::ClearScreen();
    std::cout << "Temperatura: ";
    std::cin >> this->temperature;
    this->initTemperature = this->temperature;
    this->coolingNum = 0;
    std::cout << "Wspolczynnik chlodzenia: ";
    std::cin >> this->coolingRate;
    std::cout << "Typ chlodzenia('l' - logarytmiczny, 'g' - geometryczny, 'i' - liniowy): ";
    std::cin.ignore();
    std::cin.get(this->coolingType);
    //std::cin >> this->coolingType;
    std::cout << "Typ poczatkowego rozwiazania('l' - losowe, 'n' - naturalne, 'z' - po alg. zachlannym): ";
    std::cin.ignore();
    std::cin.get(this->initSolutionType);
    //std::cin >> this->initSolutionType;
}

double AnnealingManager::GetTemperature() {
    return  this->temperature;
}

char AnnealingManager::GetInitSolutionType() {
    return  this->initSolutionType;
}

void AnnealingManager::Cool() {
    switch(this->coolingType) {
        case 'l':
            this->temperature *= 1.0 - this->coolingRate;
            break;
        case 'g':
            this->temperature /= (1+(this->coolingRate*(++this->coolingNum)));
            break;
        case 'i':
            this->temperature -= this->coolingRate;
            break;
    }
}

double AnnealingManager::GetAcceptanceProbability(int costOld, int costNew) {
    if(costOld > costNew)
        return 1.0;
    else
        return exp((costOld - costNew)/this->temperature);
}

void AnnealingManager::Reset() {
    this->temperature = this->initTemperature;
    this->coolingNum = 0;
}