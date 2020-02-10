//
// Created by lukasz on 16.10.2019.
//

#include "Menu.h"
#include "Graph.h"

#include <iostream>

using namespace std;

Menu::Menu() {
    options[0] =  " ---------------Graf-------------- \n";
    options[1] =  "|       1. Zaladuj z pliku        |\n";
    options[2] =  "|       2. Stworz recznie         |\n";
    options[3] =  "|       3. Stworz losowo          |\n";
    options[4] =  "| 4. Stworz losowo (symetrycznie) |\n";
    options[5] =  "|           5. Wyswietl           |\n";
    options[6] =  "|             6. Usun             |\n";
    options[7] =  " ------------Permutacja----------- \n";
    options[8] =  "|       7. Stworz recznie         |\n";
    options[9] =  "|       8. Stworz losowo          |\n";
    options[10] = "|     9. Wyswietl permutacje      |\n";
    options[11] = "|      10. Policz f. celu         |\n";
    options[12] = "|             11. Usun            |\n";
    options[13] = " --------Algorytmy Dokladne------- \n";
    options[14] = "|         12. Brute Force         |\n";
    options[15] = "|          13. Held Karp          |\n";
    options[16] = " ------Przeszukiwanie Lokalne------ \n";
    options[17] = "|     14. Simulated Annealing     |\n";
    options[18] = " -----------Populacyjne----------- \n";
    options[19] = "|            15. Genetic          |\n";
    options[20] = " --------------------------------- \n";
    options[21] = "|             16. Wyjdz           |\n";
    options[22] = " --------------------------------- \nWybor:";

}

void Menu::DisplayMenu() {
    for(int i=0; i<optionsNum; i++) {
        cout << this->options[i];
    }
}

void Menu::EnterMenu() {
    Graph g;
    int choice;
    int initVertex;
    srand(time(NULL));
    while(true) {
        ClearScreen();
        DisplayMenu();
        cin >> choice;
        switch (choice) {
            case 1:
                g.LoadFromFile();
                g.DisplayGraph(g.GetGraph());
                WaitForUser();
                break;
            case 2:
                g.CreateGraphManually();
                g.DisplayGraph(g.GetGraph());
                WaitForUser();
                break;
            case 3:
                g.CreateGraphRandom();
                g.DisplayGraph(g.GetGraph());
                WaitForUser();
                break;
            case 4:
                g.CreateGraphRandomSymetric();
                g.DisplayGraph(g.GetGraph());
                WaitForUser();
                break;
            case 5:
                if(g.GraphExists())
                    g.DisplayGraph(g.GetGraph());
                else
                    cout << "Nie stworzono grafu!\n";
                WaitForUser();
                break;
            case 6:
                g.EraseGraph();
                WaitForUser();
                break;
            case 7:
                if(g.GraphExists()) {
                    g.CreatePermutationManually();
                    g.DisplayPermutation(g.GetPermutation());
                }
                else
                    cout << "Najpierw nalezy utworzyc graf!\n";
                WaitForUser();
                break;
            case 8:
                if(g.GraphExists()) {
                    g.CreatePermutationRandom();
                    g.DisplayPermutation(g.GetPermutation());
                }
                else
                    cout << "Najpierw nalezy utworzyc graf!\n";
                WaitForUser();
                break;
            case 9:
                if(g.PermutationExists())
                    g.DisplayPermutation(g.GetPermutation());
                else
                    cout << "Nie stworzono permutacji!\n";
                WaitForUser();
                break;
            case 10:
                if(g.PermutationExists())
                    cout << "Wartosc funkcji celu:" <<  g.ObjectiveFunction(g.GetPermutation()) << endl;
                else
                    cout << "Najpierw nalezy utworzyc permutacje!\n";
                WaitForUser();
                break;
            case 11:
                g.ErasePermutation();
                WaitForUser();
                break;
            case 12:
                cout << "Wprowadz nr wierzcholka poczatkowego: " << endl;
                cin >> initVertex;
                if(initVertex < g.GetGraphSize() && initVertex > -1)
                    g.InitSwapBruteForce(initVertex);
                else
                    cout << "Niepoprawny wierzcholek\n";
                WaitForUser();
                break;
            case 13:
                cout << "Wprowadz nr wierzcholka poczatkowego: " << endl;
                cin >> initVertex;
                if(initVertex < g.GetGraphSize() && initVertex > -1)
                    g.HeldKarp(initVertex);
                else
                    cout << "Niepoprawny wierzcholek\n";
                WaitForUser();
                break;
            case 14:
                cout << "Wprowadz nr wierzcholka poczatkowego: " << endl;
                cin >> initVertex;
                if(initVertex < g.GetGraphSize() && initVertex > -1)
                    //g.BranchAndBound(initVertex);
                    g.SimAnnealing(initVertex);
                else
                    cout << "Niepoprawny wierzcholek\n";
                WaitForUser();
                break;
            case 15:
                g.GeneticAlgorithm();
                WaitForUser();
                break;
            case 16:
                return;
            default:
                cout << "Nie ma takiej opcji!\n";
                WaitForUser();
        }
    }
}

void Menu::WaitForUser() {
    string input;
    cout << "Nacisnij ENTER aby kontynuowac";
    cin.ignore();
    getline(cin, input);
}

void Menu::ClearScreen() {
    int numOfEndls = 20;
    for(int i=0; i<numOfEndls; i++) {
        cout<<"\n";
    }
}

Menu::~Menu() {

}