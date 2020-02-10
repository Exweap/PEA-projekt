//
// Created by exwea on 19.01.2020.
//

#ifndef PEA_GENETICMANAGER_H
#define PEA_GENETICMANAGER_H

#include <vector>

class GeneticManager {
private :
    static int populationSize;
    static int generationNum;
    static int selectionType;
    static int mutationType;
    static int crossoverType;
    static double mutationProbability;

public:
    GeneticManager();

    static int getPopulationSize();

    static int getGenerationNum();

    static int getSelectionType();

    static int getMutationType();

    static int getCrossoverType();

    static double getMutationProbability();
};


#endif //PEA_GENETICMANAGER_H
