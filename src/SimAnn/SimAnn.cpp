#include <SimAnn.hpp>
#include <Matrix.hpp>

#include <iostream>
#include <algorithm>
#include <chrono>
#include <cmath>

double SimAnn::duration_t = 0;
std::string SimAnn::neighbourhoodType = "";
double SimAnn::a_constant = 0;
int SimAnn::k_geometric = 0;

int SimAnn::paramaterSet = 0;


SimAnn::SimAnn(int size, double beginTemperature, double minTemperature){
    this->temperatureMin = minTemperature;
    this->currentValue = 0;
    this->najk = 432222222;
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda statyczna ustalająca parametry algorytmu
//--------------------------------------------------------------------------------------------------------------------------
void SimAnn::setParameters(double dur, std::string neigbType, double a, int k, int setP){
    SimAnn::duration_t = dur;
    SimAnn::neighbourhoodType = neigbType;
    SimAnn::a_constant = a;
    SimAnn::k_geometric = k;
    SimAnn::paramaterSet = setP;
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda obliczająca długość drogi na podstawie danej sekwencji wierzchołków
//--------------------------------------------------------------------------------------------------------------------------
int SimAnn::calculateCurrentValue(Matrix& matrix, std::vector<int>& tempSol){

    int value = 0;

    for(int i = 0; i < matrix.size(); i++){
        if(i == (matrix.size() - 1))
            value += matrix.matrix[tempSol[i]][tempSol[0]].value;
        else
            value += matrix.matrix[tempSol[i]][tempSol[i + 1]].value;
    }

    return value;
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda obliczająca długość drogi na podstawie danej sekwencji wierzchołków
//--------------------------------------------------------------------------------------------------------------------------
void SimAnn::calculateCurrentValueForThis(Matrix& matrix){

    int value = 0;

    for(int i = 0; i < matrix.size(); i++){
        if(i == (matrix.size() - 1))
            value += matrix.matrix[this->solution[i]][this->solution[0]].value;
        else
            value += matrix.matrix[this->solution[i]][this->solution[i + 1]].value;
    }

    this->currentValue = value;
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda, która na podstawie danego sąsiedztwa wykonuje transformację
//--------------------------------------------------------------------------------------------------------------------------
std::vector<int> SimAnn::neighbourhood(){

    std::vector<int> sol = this->solution;

    std::string userInput = this->neighbourhoodType;
    std::transform(neighbourhoodType.begin(), neighbourhoodType.end(), neighbourhoodType.begin(), ::tolower);
    
    if(userInput == "swap"){ // Użytkownik wybrał sąsiedztwo typu "swap"
        int randIndexA, randIndexB;

        randIndexA = rand()%sol.size(); // losujemy pierwszy indeks
        do{
            randIndexB = rand()%sol.size(); // losujemy drugi indeks
        }while(randIndexB == randIndexA); // indeksy nie mogą być takie same

        // zamiana elementów
        int temp;
        temp = sol[randIndexB];
        sol[randIndexB] = sol[randIndexA];
        sol[randIndexA] = temp;
    }
    else if(userInput == "invert"){ // Użytkownik wybrał sąsiedztwo typu "invert"
        int randIndexA, randIndexB;

        randIndexA = rand()%sol.size(); // losujemy pierwszy indeks
        do{
            randIndexB = rand()%sol.size(); // losujemy drugi indeks
        }while(randIndexB == randIndexA); // indeksy nie mogą być takie same

        if(randIndexA > randIndexB){ // indeks A ma być mniejszy niż B
            int temp = randIndexA;
            randIndexA = randIndexB;
            randIndexB = temp;
        }

        int left = randIndexA;
        int right = randIndexB;

        while(left < right){ // Wykonujemy zamianę elementów pomiędzy wybranym zakresem [left, right]
            int temp = sol[left];
            sol[left] = sol[right];
            sol[right] = temp;
            left ++;
            right --;
        }
    }

    return sol;
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda, która realizuje schemat chłodzenia
//--------------------------------------------------------------------------------------------------------------------------
void SimAnn::cooling(){
    this->temperature *= this->a_constant;
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda główna w klasie. Realizuje algorytm Simulated Annealing na podstawie przedstawionego schematu działania algorytmu
// na wykładzie
//--------------------------------------------------------------------------------------------------------------------------
void SimAnn::algorithm(Matrix& matrix){

    if(SimAnn::paramaterSet != 1){ // jeśli użytkownik nie ustawił parametrów
        std::cout << "You need to set parameters before the algorithm starts";
        std::cout << std::endl;
        return;
    }
    else{

        // inicjalizacja sekwencji początkowej
        int array[matrix.size()];

        for(int i = 0; i < matrix.size(); i++)
            array[i] = i;

        std::random_shuffle(array, array + matrix.size());

        for(int i = 0; i < matrix.size(); i++)
            this->solution.push_back(array[i]);

        calculateCurrentValueForThis(matrix); // dla początkowej sekwencji obliczamy droge
        this->temperature = this->currentValue * 10; // wyznaczamy temperature poczatkowa
        std::cout << std::endl << "Initial temperature: " << this->temperature << std::endl;

        double time_length;
        std::chrono::duration<double> elapsed_seconds; // czas, który upłynął
        std::chrono::time_point<std::chrono::system_clock> start_alg, end_alg; // początek i koniec odmierzania czasu

        start_alg = std::chrono::system_clock::now(); // rozpoczynamy mierzenie czasu wykonania algorytmu
        elapsed_seconds = start_alg - start_alg;

        std::vector<int> tempSolution; // nowe potencjalne rozwiązanie y

        int age = (matrix.size()*(matrix.size()-1))/2*0.25; // wyznaczamy długość epoki
        double k_const = 1;

        while(elapsed_seconds.count() < duration_t){ // warunek stopu

            for(int i = 0; i < age; i++){
                tempSolution = neighbourhood(); // algorytm realizujacy transformacje na zadanej sekwencji

                int wayValueTemp;
                
                calculateCurrentValueForThis(matrix); // dla dotychczasowego rozwiązania obliczamy drogę
                wayValueTemp = calculateCurrentValue(matrix, tempSolution); // dla nowej sekwencji obliczamy droge

                if((wayValueTemp - this->currentValue) < 0){ // jeśli nowe rozwiązanie jest lepsze, zaakceptuj je
                    this->solution = tempSolution;
                    this->currentValue = wayValueTemp;
                }
                else{ // w przeciwnym przypadku
                    double randomValueToCompare = ((double) std::rand() / (RAND_MAX)); // wylosuj s z zakresu [0,1]

                    // przyjmij rozwiązanie jeśli jest one mniejsze niż prawdopodbieństwo zadane wzorem takim jak dla teorii SA
                    if(randomValueToCompare < exp(-(wayValueTemp - this->currentValue)/temperature)){
                        this->solution = tempSolution;
                        this->currentValue = wayValueTemp;
                    }
                }
            }

            if(this->currentValue < this->najk) // Sprawdź, czy jest to do tej pory najlepsze znalezione rozwiązanie
                this->najk = this->currentValue;

            cooling(); // obniżanie temperatury
            end_alg = std::chrono::system_clock::now(); // koniec pomiaru czasu
            elapsed_seconds = end_alg - start_alg; // ilość czasu, która upłyneła od uruchomienia

            if(elapsed_seconds.count() > (5 * k_const) && this->duration_t >= 55){ // dodaj częśćiowe rozwiązania na listę
                partsolutionvalues.push_back(this->currentValue);
                k_const ++;
            }

        }
        std::cout << "Final temperature: " << this->temperature << std::endl << std::endl;
    }
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda wypisująca wynik działania algorytmu
//--------------------------------------------------------------------------------------------------------------------------
void SimAnn::showResult(){
    std::cout << std::endl;
    std::cout << "Found way: " << std::endl;

    do{ // Zacznij wypisywanie drogi od wierzchołka '0'
        int last = this->solution[this->solution.size() - 1];

        for(int i = (this->solution.size() - 1); i >= 0; i--){
            if((i - 1) >= 0)
                this->solution[i] = this->solution[i - 1];
            else
                this->solution[i] = last;
        }
    }while(this->solution[0] != 0);

    for(int i = 0; i < this->solution.size(); i++){ // wypisanie sekwencji
        if(i == this->solution.size() - 1)
            std::cout << this->solution[i];
        else
            std::cout << this->solution[i] << " - ";
    }
    std::cout << std::endl;

    std::cout << "Value: "; // wynikowa droga
    std::cout << this->currentValue;
    std::cout << std::endl << std::endl;

    if(this->duration_t >= 55){ // wypisanie częściowych rozwiązań (czas domyślny- 60s)
        std::cout << "Solutions after period of time: " << std::endl;
        for(int i = 1; i <= 9; i++){
            std::cout << "Time [" << i*5 << "s]: " << partsolutionvalues[i - 1] << std::endl;
        }
    }
}
