//
// Created by lukasz on 16.10.2019.
//
#include "Graph.h"

Graph::Graph() {
    this->minPath = INT_MAX;
}

/*
 * Funkcja wczytująca instancję grafu z pliku
 * Na początku następuje wyczyszczenie poprzednio załadowanej instancji problemu (jeśli takowa istniała)
 * Wczytywana linia jest przekazywana do stringa, a następnie z użyciem biblioteki stringstream konwertowana do wektora
*/
void Graph::LoadFromFile() {
    //ErasePermutation();
    this->EraseGraph();
    std::ifstream inFile;
    std::string fileName;
    std::cout << "Wprowadz nazwe pliku tekstowego, z ktorego wczytana zostanie instancja grafu (bez rozszerzenia '.txt')\n";
    std::cin >> fileName;
    fileName += ".txt";
    inFile.open(fileName);
    if(!inFile) {
        std::cout << "Wystapil blad otwierania pliku, sprawdz poprawnosc nazwy!\n";
        return;
    }
    int i=0;
    std::string line;
    while(std::getline(inFile, line)) {
        std::vector<int> lineData;
        std::stringstream lineStream(line);
        int value;
        while(lineStream >> value) {
            lineData.push_back(value);
        }
        this->graph.push_back(lineData);
        i++;

    }
    inFile.close();
}

/*
 * Funkcja umożliwiwająca własnoręczne stworzenie instancji grafu
 * Na początku należy podać ilość wierzchołków tworzonego grafu, następnie koszty przejść z wierzchołków od 0 do N
 * do wszystkich pozostałych wierzchołków
 * Po wczytaniu linii z kosztami przejść funkcja umieszcza wartość "-1" w miejscu odpowiadającym
 * przejściu pomiędzy tym samym wierzchołkiem
 */
void Graph::CreateGraphManually() {
    ErasePermutation();
    this->EraseGraph();
    int verticesNum;
    std::cout << "Wprowadz liczbe wierzcholkow: \n";
    std::cin >> verticesNum;
    std::cin.ignore();
    if(verticesNum < 2) {
        std::cout << "Ilosc wierzcholkow jest zbyt mala\n";
        return;
    }
    for(int i=0; i<verticesNum; i++) {
        std::string row;
        std::cout << "Wprowadz koszty przejsc z wierzcholka " << i << " do wierzcholkow ";// << (i+1)%verticesNum << ", " << (i+2)%verticesNum << ", " << (i+3)%verticesNum << " i " << (i+4)%verticesNum << " oddzielajac wartości klawiszem SPACE" <<endl;
        for(int j=0; j<verticesNum; j++) {
            if(j == i)
                continue;
            else
                std::cout << j <<", ";
        }
        std::cout << "oddzielajac wartości klawiszem SPACE\n";
        getline(std::cin, row);
        std::vector<int> lineData;
        std::stringstream lineStream(row);
        int value;
        while(lineStream >> value)
            lineData.push_back(value);
        lineData.insert(lineData.begin()+i, -1);
        this->graph.push_back(lineData);
    }
}

/*
 * Losowa generacja instancji grafu
 * Możliwa jest generacja instancji losowej symetrycznej lub też zupełnie losowej na podstawie domyślnego atrybutu "sym"
 */
void Graph::CreateGraphRandom(bool sym) {
    ErasePermutation();
    this->EraseGraph();
    int verticesNum;
    std::cout << "Wprowadz liczbe wierzcholkow w celu wygenerowania grafu: \n";
    std::cin >> verticesNum;
    if(verticesNum < 2) {
        std::cout << "Ilosc wierzcholkow jest zbyt mala\n";
        return;
    }
    srand(time(nullptr));
    for(int i=0; i<verticesNum; i++) {
        std::vector<int> lineData;
        if(sym) {
            lineData.push_back(-1);
            for(int j=i; j<verticesNum-1; j++) {
                lineData.push_back(1+rand()%100);
            }
            for(int j=0; j<i; j++) {
                lineData.insert(lineData.begin()+j, this->graph[j][i]);
            }
        }
        else {
            for (int j = 0; j < verticesNum - 1; j++)
                lineData.push_back(1 + rand() % 100);
            lineData.insert(lineData.begin() + i, -1);
        }
        this->graph.push_back(lineData);
    }
}

/*
 * Symetryczna losowa generacja grafu
 * Funkcja wywołuje CreateGraphRandom i nadpisuje domyślny atrybut umożliwiając utworzenie macierzy symetrycznej
 * względem przekątnej
 */
void Graph::CreateGraphRandomSymetric() {
    CreateGraphRandom(true);
}

/*
 * Funkcja umożliwiająca stworzenie permutacji w sposób ręczny
 * Permutacja powinna składać się z wszystkich wierzchołków utworzonego grafu
 * Pod koniec działania funkcji automatycznie dodawany do permutacji jest jej pierwszy element,
 * który umożliwia powrót do punktu startowego
 */
void Graph::CreatePermutationManually() {
    ErasePermutation();
    std::cout << "Struktura zaladowanego grafu: \n";
    DisplayGraph(this->graph);
    std::cout << "\nWprowadz permutacje oddzielajac kolejny elementy klawiszem SPACE (permutacja musi miec ilosc elementow identyczna do ilosci wierzcholkow w grafie): \n";
    std::string row;
    getline(std::cin.ignore(), row);
    std::stringstream lineStream(row);
    int value;
    while(lineStream >> value)
        this->permutation.push_back(value);
    //int front = this->permutation.front();
    //this->permutation.push_back(front);
    if(this->permutation.size() > this->graph.size() || this->permutation.size() < this->graph.size()) {
        std::cout << "Podana permutacja jest niepoprawna!\n";
        ErasePermutation();
    }
}

/*
 * Funkcja losowej generacji permutacji
 */
void Graph::CreatePermutationRandom() {
    ErasePermutation();
    std::vector<int>temp;
    temp.reserve(this->graph.size());
    for(int i=0; i<this->graph.size(); i++)
        temp.push_back(i);
    for(int i=0; i<this->graph.size(); i++) {
        int drawn = rand()%(this->graph.size()-i);
        this->permutation.push_back(temp[drawn]);
        temp.erase(temp.begin()+drawn);
    }
    //int front = this->permutation.front();
    //this->permutation.push_back(front);
    /*if(this->permutation.size() > this->graph.size()+1 || this->permutation.size() < this->graph.size()+1) {
        cout << "Podana permutacja jest niepoprawna!" << endl;
        this->ErasePermutation();
    }*/
}

std::vector<int> Graph::CreatePermutationRand() {
    std::vector<int>temp, permutation;
    temp.reserve(this->graph.size());
    for(int i=0; i<this->graph.size(); i++)
        temp.push_back(i);
    for(int i=0; i<this->graph.size(); i++) {
        int drawn = rand()%(this->graph.size()-i);
        permutation.push_back(temp[drawn]);
        temp.erase(temp.begin()+drawn);
    }
    return permutation;
}

/*
 * Metoda obliczająca wartość funkcji celu dla zapisanej permutacji
 */
int Graph::ObjectiveFunction(std::vector<int> permutation) {
    int pathLength=0;
    int n = this->graph.size();
    for(int i=0; i<n-1; i++)
        pathLength+=this->graph[permutation[i]][permutation[i+1]];
    pathLength+=this->graph[permutation[n-1]][permutation[0]];
    return pathLength;
}

void Graph::DisplayGraph(std::vector<std::vector<int> > graph) {
    for(int i=0; i<graph.size(); i++) {
        for(int j=0; j<graph[i].size(); j++)
            std::cout << graph[i][j] << " ";
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Graph::DisplayPermutation(std::vector<int> permutation) {
    for(int i=0; i<permutation.size(); i++)
        std::cout << permutation[i] << " ";
    std::cout << "\n";
}

std::vector<int> Graph::GetPermutation()
{
    return this->permutation;
}

std::vector<std::vector<int> > Graph::GetGraph() {
    return this->graph;
}

int Graph::GetGraphSize() {
    return this->graph.size();
}

bool Graph::GraphExists() {
    return this->graph.size() > 1;
}

bool Graph::PermutationExists() {
    if(this->permutation.size()>2)
        return true;
    else
        return false;
}

void Graph::EraseGraph() {
    int len=this->graph.size();
    this->graph.erase(this->graph.begin(), this->graph.begin() + len);
}

void Graph::ErasePermutation() {
    int len=this->permutation.size();
    if(len != 0)
        this->permutation.erase(this->permutation.begin(), this->permutation.begin() + len);
    len=this->minPermutation.size();
    if(len != 0)
        this->minPermutation.erase(this->permutation.begin(), this->permutation.begin() + len);
}

Graph::~Graph() = default;
