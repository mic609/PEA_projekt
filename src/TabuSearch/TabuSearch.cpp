#include <TabuSearch.hpp>

#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <fstream>

double TabuSearch::duration_t = 0;
std::string TabuSearch::neighbourhoodType = "";
int TabuSearch::diversification = 0;

int TabuSearch::paramaterSet = 0;

TabuSearch::TabuSearch(){
    this->currentValue = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda statyczna ustalająca parametry algorytmu
//--------------------------------------------------------------------------------------------------------------------------
void TabuSearch::setParameters(double dur, std::string neigbType, int diver, int setP){
    TabuSearch::duration_t = dur;
    TabuSearch::neighbourhoodType = neigbType;
    TabuSearch::diversification = diver;
    TabuSearch::paramaterSet = setP;
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda obliczająca długość drogi na podstawie danej sekwencji wierzchołków
//--------------------------------------------------------------------------------------------------------------------------
int TabuSearch::calculateCurrentValue(Matrix& matrix, std::vector<int>& tempSol){

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
void TabuSearch::calculateCurrentValueForThis(Matrix& matrix){

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
std::vector<int> TabuSearch::neighbourhood(int IndexA, int IndexB){

    std::vector<int> sol = this->solution;

    std::string userInput = this->neighbourhoodType;
    std::transform(neighbourhoodType.begin(), neighbourhoodType.end(), neighbourhoodType.begin(), ::tolower);
    
    if(userInput == "swap"){ // Użytkownik wybrał sąsiedztwo typu "swap"
        // zamiana elementów
        int temp;
        temp = sol[IndexB];
        sol[IndexB] = sol[IndexA];
        sol[IndexA] = temp;
    }
    else if(userInput == "invert"){ // Użytkownik wybrał sąsiedztwo typu "invert"

        int left = IndexA;
        int right = IndexB;

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
// Metoda, która przeszukuje sasiedztwo w poszukiwaniu najlepszego rozwiazania
//--------------------------------------------------------------------------------------------------------------------------
std::vector<int> TabuSearch::neighbourhoodLook(Matrix& matrix){

    std::vector<int> bestvector = this->proposedSolution;
    int bestvalue = calculateCurrentValue(matrix, bestvector);

    std::string userInput = this->neighbourhoodType;
    std::transform(neighbourhoodType.begin(), neighbourhoodType.end(), neighbourhoodType.begin(), ::tolower);
    
    if(userInput == "swap"){ // Użytkownik wybrał sąsiedztwo typu "swap"
        int IndexA, IndexB;
        int i = 0;

        std::vector<int> sol = bestvector;
        int valueA = calculateCurrentValue(matrix, bestvector); // dlugosc sciezki dla dotychczasowej sekwencji

        for(int IndexA = i; IndexA < (this->solution.size() - 1); IndexA ++){
            for(int IndexB = i + 1; IndexB < this->solution.size(); IndexB ++){
                
                // zamiana elementów
                int temp;
                temp = bestvector[IndexB];
                bestvector[IndexB] = bestvector[IndexA];
                bestvector[IndexA] = temp;
                
                // po zamianie elementow sprawdzamy dlugosc sciezki dla nowej sekwencji
                int valueB;
                valueB = calculateCurrentValue(matrix, bestvector);

                // Jesli wybrany ruch typu swap nie jest na liscie ruchow zakazanych i
                // jesli po zamianie elementow otrzymalismy sekwencje, dla ktorej droga jest krotsza, to
                // zapisz nowe najlepsze rozwiazanie z sasiedztwa i dokonany ruch. Wprowadzono dodatkowo kryterium aspiracji:
                // jesli ruch jest zakazany ale daje nowe najlepsze globalne rozwiazanie, to je zaakceptuj
                if((valueB < valueA && tabulist[IndexA][IndexB] == 0)||(valueB < this->currentValue)){
                    proposedSolution = bestvector;
                    bestvalue = valueB;
                    best_from = IndexA;
                    best_to = IndexB;
                }
                else
                    bestvector = sol;
            }
        }
    }
    else if(userInput == "invert"){ // Użytkownik wybrał sąsiedztwo typu "invert"
        int IndexA, IndexB;
        int i = 0;

        std::vector<int> sol = bestvector;
        int valueA = calculateCurrentValue(matrix, bestvector); // dlugosc sciezki dla dotychczasowej sekwencji

        for(int IndexA = i; IndexA < (this->solution.size() - 1); IndexA ++){
            for(int IndexB = i + 1; IndexB < this->solution.size(); IndexB ++){
                
                int left = IndexA;
                int right = IndexB;

                while(left < right){ // Wykonujemy zamianę elementów pomiędzy wybranym zakresem [left, right]
                    int temp = bestvector[left];
                    bestvector[left] = bestvector[right];
                    bestvector[right] = temp;
                    left ++;
                    right --;
                }
                
                // po zamianie elementow sprawdzamy dlugosc sciezki dla nowej sekwencji
                int valueB;
                valueB = calculateCurrentValue(matrix, bestvector);

                // Analogicznie jak dla ruchu typu swap (komentarz w 127 linii kodu)
                if((valueB < valueA && tabulist[IndexA][IndexB] == 0)||(valueB < this->currentValue)){
                    proposedSolution = bestvector;
                    bestvalue = valueB;
                    best_from = IndexA;
                    best_to = IndexB;
                }
                else
                    bestvector = sol;
            }
        }
    }
    proposedValue = bestvalue;
    return {best_from, best_to}; // zwracamy ruch, ktory wygenerowal najlepsze rozwiazanie w sasiedztwie
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda główna w klasie
//--------------------------------------------------------------------------------------------------------------------------
void TabuSearch::algorithm(Matrix& matrix){
    if(TabuSearch::paramaterSet != 1){ // jeśli użytkownik nie ustawił parametrów
        std::cout << "You need to set parameters before the algorithm starts";
        std::cout << std::endl;
        return;
    }
    else{
        tabucount = 0; // inicjalizacja dlugosci listy tabu
        double period = this->duration_t/16; // okres, co jaki brane jest rozwiazanie czesciowe (na potrzeby testow)

        // inicjalizacja sekwencji początkowej
        int array[matrix.size()];

        for(int i = 0; i < matrix.size(); i++)
            array[i] = i;

        std::random_shuffle(array, array + matrix.size());

        for(int i = 0; i < matrix.size(); i++)
            this->solution.push_back(array[i]);

        calculateCurrentValueForThis(matrix); // dla początkowej sekwencji obliczamy droge

        //inicjalizacja listy tabu
        for(int i = 0; i < matrix.size(); i++){
            std::vector<int> add_to_vector(matrix.size());
            tabulist.push_back(add_to_vector);
            for(int j = 0; j < matrix.size(); j++){
                tabulist[i].push_back(j);
                tabulist[i][j] = 0;
            }
        }

        double time_length;
        std::chrono::duration<double> elapsed_seconds; // czas, który upłynął
        std::chrono::time_point<std::chrono::system_clock> start_alg, end_alg; // początek i koniec odmierzania czasu

        start_alg = std::chrono::system_clock::now(); // rozpoczynamy mierzenie czasu wykonania algorytmu
        elapsed_seconds = start_alg - start_alg;

        //std::vector<int> tempSolution;

        int found = 1; // znaleziono / nie znaleziono rozwiazania
        int iter = 0; // liczba iteracji petli glownej
        this->proposedSolution = this->solution; // inicjalizacja rozwiazania potencjalnego
        this->proposedValue = this->currentValue;
        double k_const = 1; 

        while(elapsed_seconds.count() < duration_t){ // warunek stopu

            std::vector<int> move = neighbourhoodLook(matrix); // znajdz ruch, ktory wygeneruje najlepsze rozwiazanie w sasiedztwie

            if((this->proposedValue - this->currentValue) < 0){ // jeśli nowe rozwiązanie jest lepsze, zaakceptuj je jako optymalne
                this->solution = this->proposedSolution;
                this->currentValue = this->proposedValue;
                found = 1;
                iter = 0;
            }

            if(tabucount <= (matrix.size()/2)){ // ograniczona dlugosc listy tabu
                tabulist[move[0]][move[1]] = matrix.size() *2; // wartosc kadencji stala
                tabulist[move[1]][move[0]] = matrix.size() *2; // wartosc kadencji stala
                tabucount ++;
            }

            // Aktualizacja listy tabu, zmniejszanie dlugosci kadencji po iteracji petli
            for (int i = 0; i < matrix.size(); i++) {
                for (int j = 0; j < matrix.size(); j++) {
                    if (tabulist[i][j] > 0) {
                        tabulist[i][j]--;
                        if(tabulist[i][j] == 0)
                            tabucount --;
                    }
                }
            }

            // Dywersyfikacja. Jesli algorytm przeszedl 100 iteracji petli odkad znalazl ostatnie rozwiazanie, to
            // wylosuj nowe potencjalne rozwiazanie
            if(iter >= 100 && found == 1 && diversification == 1){
                
                int arr[matrix.size()];

                for(int i = 0; i < matrix.size(); i++)
                    arr[i] = i;

                std::random_shuffle(arr, arr + matrix.size());

                for(int i = 0; i < matrix.size(); i++)
                    this->proposedSolution[i] = arr[i];

                iter = 0;

                this->proposedValue = calculateCurrentValue(matrix, proposedSolution);
            }
            end_alg = std::chrono::system_clock::now(); // koniec pomiaru czasu
            elapsed_seconds = end_alg - start_alg; // ilość czasu, która upłyneła od uruchomienia

            if(elapsed_seconds.count() > (period * k_const) && this->duration_t >= (period * 15)){ // dodaj częśćiowe rozwiązania na listę
                partsolutionvalues.push_back(this->currentValue);
                k_const ++;
            }

            iter ++;
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda wypisująca wynik działania algorytmu
//--------------------------------------------------------------------------------------------------------------------------
void TabuSearch::showResult(){
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
    std::cout << " - 0" << std::endl;

    std::cout << "Value: "; // wynikowa droga
    std::cout << this->currentValue;
    std::cout << std::endl << std::endl;

    double period = this->duration_t/16;

    if(this->duration_t >= period * 15){ // wypisanie częściowych rozwiązań
        std::cout << "Solutions after period of time: " << std::endl;
        for(int i = 1; i <= 15; i++){
            std::cout << "Time [" << i*period << "s]: " << partsolutionvalues[i - 1] << std::endl;
        }
    }
}
