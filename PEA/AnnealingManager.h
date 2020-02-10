//
// Created by exwea on 09.12.2019.
//

#ifndef PEA_ANNEALINGMANAGER_H
#define PEA_ANNEALINGMANAGER_H


class AnnealingManager {
private:
    double temperature;
    double initTemperature;
    double coolingRate;
    char coolingType;
    char initSolutionType;
    int coolingNum;
public:
    AnnealingManager();
    double GetTemperature();
    char GetInitSolutionType();
    void Cool();
    double GetAcceptanceProbability(int costOld, int costNew);
    void Reset();
};


#endif //PEA_ANNEALINGMANAGER_H
