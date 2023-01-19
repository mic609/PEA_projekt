#include <Genetic.hpp>
#include <Matrix.hpp>
#include <chrono>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <fstream>

double Genetic::crossProbability = 0.0; // prawdopodobienstwo krzyzowania
double Genetic::mutationProbability = 0.0; // prawdopodobienstwo mutacji
std::string Genetic::mutationType = ""; // typ mutacji
int Genetic::populationSize = 0; // wielkosc populacji
double Genetic::algDuration = 0.0; // kryterium stopu
int Genetic::paramaterSet = 0;

Genetic::Genetic(){
}

void Genetic::populationInit(Matrix& matrix){
    std::vector<int> randomPath;

    for(int i = 0; i < matrix.size(); i++)
        randomPath.push_back(i);

    for (int i = 0; i < this->populationSize; i++)
		{
			std::random_shuffle(randomPath.begin(), randomPath.end());
			Chromosome chromosome;

            for(int i = 0; i < randomPath.size(); i++)
                chromosome.path.push_back(randomPath[i]);

            chromosome.pathLength = calculateCurrentValue(matrix, randomPath);
			population.push_back(chromosome);
		}
}

void Genetic::rouletteSelection(){

    if(!populationGeneric.empty())
        populationGeneric.clear();

    // double totalFitness = 0;

    // // obliczamy sume funkcji przystosowania wszystkich osobnikow
    // for (auto& ind : population)
    //     totalFitness += ind.fitness;

    // double randomNumber = ((double)rand() * totalFitness) / (double)RAND_MAX;
    // totalFitness = 0;
    // for (int i = 0; i < population.size(); i++)
    // {
    //     totalFitness += population[i].fitness;
    //     if (totalFitness >= randomNumber){
    //         populationGeneric.push_back(population[i];)
    //     }
    // }

    double total_fitness = 0; // suma jakosci wszystkich rozwiazan
    int population_size = population.size();

    double fitness[population.size()];
    int i = 0;

    // Sumowanie długości trasy całej populacji
    for (Chromosome ind : population){
        fitness[i] = 1.0/(double)ind.pathLength; // ocena
        total_fitness += fitness[i];
        i++;
    }

    for(int j = 0; j < (population.size())*0.3; j++){
        double f = (double)rand() / RAND_MAX;
        double random_value = f * (total_fitness);
        double fitness_sum = 0;
        i = 0;
        for (Chromosome ind : population) {
            fitness_sum += fitness[i];
            if (fitness_sum >= random_value) {
                populationGeneric.push_back(ind);
                break;
            }
            i++;
        }
    }
}

void Genetic::davisCrossover(Matrix& matrix){

    int counter = 0;

    while (counter < (populationGeneric.size() - 1)){
        // std::cout << "counter: " << counter << std::endl;

        double randomValue = ((double)rand() / (RAND_MAX));

        // std::cout << "Sciezka wejsciowa (rodzic 1): ";
        // for(int vertex: populationGeneric[counter].path){
        //     std::cout << vertex << " ";
        // }
        // std::cout <<std::endl<<std::endl;
        // std::cout << "Sciezka wejsciowa (rodzic 2): ";
        // for(int vertex: populationGeneric[counter + 1].path){
        //     std::cout << vertex << " ";
        // }
        // std::cout <<std::endl<<std::endl;

        Chromosome child1, child2;

        if (randomValue < crossProbability)
        {
            int cross_point1;
            int cross_point2;
            do{
                cross_point1 = rand() % (matrix.size() - 1);
            }while (cross_point1 == 0);
            do{
                cross_point2 = rand() % (matrix.size() - 1);
            }while ((cross_point1 == cross_point2) || (cross_point2 == 0));

            //upewnienie się, że pierwszy punkt jest mniejszy niż drugi
            if (cross_point1 > cross_point2) {
                std::swap(cross_point1, cross_point2);
            }

            // std::cout << "cross1: " << cross_point1 << std::endl;
            // std::cout << "cross2: " << cross_point2 << std::endl;

            Chromosome child1, child2;
            std::unordered_set<int> child1_set;
            std::unordered_set<int> child2_set;

            for(int i = 0; i < matrix.size(); i++){
                child1.path.push_back(i);
                child2.path.push_back(i);
            }

            for (int i = cross_point1; i <= cross_point2; i++) {
                child1.path[i] = populationGeneric[counter].path[i];
                child1_set.insert(populationGeneric[counter].path[i]);
                child2.path[i] = populationGeneric[counter + 1].path[i];
                child2_set.insert(populationGeneric[counter + 1].path[i]);
            }

            int current_index1 = cross_point2 + 1;
            if(current_index1 == matrix.size())
                current_index1 = 0;
            int current_index2 = cross_point2 + 1;
            if(current_index2 == matrix.size())
                current_index2 = 0;

            for (int i = current_index1; i != cross_point1; i++) {

                if(i == matrix.size())
                    i = 0;

                while (child1_set.count(populationGeneric[counter + 1].path[current_index1]) != 0) {
                    current_index1++;
                    if(current_index1 == matrix.size())
                        current_index1 = 0;
                    //std::cout << "petla" << std::endl;
                }
                child1.path[i] = populationGeneric[counter + 1].path[current_index1];
                child1_set.insert(populationGeneric[counter + 1].path[current_index1]);
                if(current_index1 == (matrix.size())-1)
                        current_index1 = 0;
                else
                    current_index1++;
            }

            for (int i = current_index2; i != cross_point1; i++) {

                if(i == matrix.size())
                    i = 0;

                while (child2_set.count(populationGeneric[counter].path[current_index2]) != 0) {
                    current_index2++;
                    if(current_index2 == matrix.size())
                        current_index2 = 0;
                    //std::cout << "petla" << std::endl;
                }
                child2.path[i] = populationGeneric[counter].path[current_index2];
                child2_set.insert(populationGeneric[counter].path[current_index2]);
                if(current_index2 == (matrix.size())-1)
                    current_index2 = 0;
                else
                    current_index2++;
            }

            child1.pathLength = calculateCurrentValue(matrix, child1.path);
            child2.pathLength = calculateCurrentValue(matrix, child2.path);

            population.push_back(child1);
            population.push_back(child2);
            counter = counter + 2;

            // std::cout << "Sciezka wyjsciowa (dziecko 1): ";
            // for(int vertex: child1.path){
            //     std::cout << vertex << " ";
            // }
            // std::cout <<std::endl<<std::endl;
            // std::cout << "Sciezka wyjsciowa (dziecko 2): ";
            // for(int vertex: child2.path){
            //     std::cout << vertex << " ";
            // }
        }
        else
        {
            counter = counter + 2;
        }
    }
    populationGeneric.clear();
}

void Genetic::mutate(Matrix& matrix){
    for (int i = 0; i < population.size(); i++){
        double randomValue = ((double)rand() / (RAND_MAX));
        if (randomValue < mutationProbability){
            if (mutationType == "transposition"){
                int index1 = rand() % population[i].path.size();
                int index2;
                do{
                    index2 = rand() % population[i].path.size();
                } while (index2 == index1);

                int temp = population[i].path[index1];
                population[i].path[index1] = population[i].path[index2];
                population[i].path[index2] = temp;

                population[i].pathLength = calculateCurrentValue(matrix, population[i].path);
            }
            else if (mutationType == "inversion"){
                int index1 = rand() % population[i].path.size();
                int index2;
                do
                {
                    index2 = rand() % population[i].path.size();
                } while (index2 == index1);
                if (index2 < index1)
                {
                    int tmp = index2;
                    index2 = index1;
                    index1 = tmp;
                }
                std::reverse(population[i].path.begin() + index1, population[i].path.begin() + index2 + 1);
                population[i].pathLength = calculateCurrentValue(matrix, population[i].path);
            }
        }
    }
}

void Genetic::elite(){
    int index = population.size() - 1;
    while(index > (populationSize - 1)){
        population.pop_back();
        index --;
    }
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda główna w klasie. Realizuje algorytm genetyczny
//--------------------------------------------------------------------------------------------------------------------------
void Genetic::algorithm(Matrix& matrix){
    if(Genetic::paramaterSet != 1){ // jeśli użytkownik nie ustawił parametrów
        std::cout << "You need to set parameters before the algorithm starts";
        std::cout << std::endl;
        return;
    }
    else{
        double period = this->algDuration/16;
        double k_const = 1;

        double time_length;
        std::chrono::duration<double> elapsed_seconds; // czas, który upłynął
        std::chrono::time_point<std::chrono::system_clock> start_alg, end_alg; // początek i koniec odmierzania czasu

        populationInit(matrix); // generujemy populacje poczatkowa. Uzupelniamy wektor "population"
        
        std::sort(population.begin(), population.end(), [](const Chromosome &a, const Chromosome &b){
            return a.pathLength < b.pathLength;
        }); // sortujemy

        bestSolution = population[0];

        // std::cout << "Populacja:  " <<std::endl;
        // int i = 0;
        // for (Chromosome ind : population) {
        //     std::cout << "Osobnik " << i << ": " << std::endl;
        //     std::cout << ind.pathLength << std::endl;
        //     i++;
        // }
        // std::cout << std::endl;

        // i = 0;
        // std::cout << "Populacja macierzysta:  " <<std::endl;
        // for (Chromosome ind : populationGeneric) {
        //     std::cout << "Osobnik " << i << ": " << std::endl;
        //     std::cout << ind.pathLength << std::endl;
        //     i++;
        // }

        start_alg = std::chrono::system_clock::now(); // rozpoczynamy mierzenie czasu wykonania algorytmu
        elapsed_seconds = start_alg - start_alg;

        while(elapsed_seconds.count() < algDuration){
            // std::cout << "Populacja: " << population.size() << std::endl;
            // std::cout << "PopulacjaGeneric: " << populationGeneric.size() << std::endl;
            rouletteSelection();
            // if(populationGeneric.size() < 2){
            //     population.clear();
            //     populationInit(matrix);
            //     std::sort(population.begin(), population.end(), [](const Chromosome &a, const Chromosome &b){
            //         return a.pathLength < b.pathLength;
            //     }); // sortujemy
            //     rouletteSelection();
            // }
            // std::cout << "PopulacjaGeneric after roulette: " << populationGeneric.size() << std::endl;
            davisCrossover(matrix);
            // std::cout << "Populacja po calym cyklu: " << population.size() << std::endl << std::endl;

            std::sort(population.begin(), population.end(), [](const Chromosome &a, const Chromosome &b){
                return a.pathLength < b.pathLength;
            }); // sortujemy

            mutate(matrix);
            elite();

            if (bestSolution.pathLength > population[0].pathLength){
                for(int i = 0; i < matrix.size(); i++)
                    bestSolution.path[i] = population[0].path[i];

                bestSolution.pathLength = population[0].pathLength;
            }

            // std::cout << "best solution: " << bestSolution.pathLength << std::endl;

            end_alg = std::chrono::system_clock::now(); // koniec pomiaru czasu
            elapsed_seconds = end_alg - start_alg; // ilość czasu, która upłyneła od uruchomienia

            if(elapsed_seconds.count() > (period * k_const) && this->algDuration >= period * 15){ // dodaj częśćiowe rozwiązania na listę
                partsolutionvalues.push_back(this->bestSolution.pathLength);
                k_const ++;
            }
            generationCounter++;
            // std::cout << "liczba osobnikow: " << population.size() << std::endl;
        }
    }

    std::cout << std::endl << "Liczba pokolen: " << generationCounter << std::endl;
    generationCounter = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda statyczna ustalająca parametry algorytmu
//--------------------------------------------------------------------------------------------------------------------------
void Genetic::setParameters(double cross, double mut, int popsize, std::string muttype, double dur, int setP){
    Genetic::algDuration = dur;
    Genetic::crossProbability = cross;
    Genetic::mutationProbability = mut;
    Genetic::populationSize = popsize;
    Genetic::mutationType = muttype;
    Genetic::paramaterSet = setP;
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda obliczająca długość drogi na podstawie danej sekwencji wierzchołków
//--------------------------------------------------------------------------------------------------------------------------
int Genetic::calculateCurrentValue(Matrix& matrix, std::vector<int>& tempSol){

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
// Metoda wypisująca wynik działania algorytmu
//--------------------------------------------------------------------------------------------------------------------------
void Genetic::showResult(){
    std::ofstream file;
    file.open("wyniki.txt", std::ios_base::app);

    std::cout << std::endl;
    std::cout << "Found way: " << std::endl;

    do{ // Zacznij wypisywanie drogi od wierzchołka '0'
        int last = this->bestSolution.path[this->bestSolution.path.size() - 1];

        for(int i = (this->bestSolution.path.size() - 1); i >= 0; i--){
            if((i - 1) >= 0)
                this->bestSolution.path[i] = this->bestSolution.path[i - 1];
            else
                this->bestSolution.path[i] = last;
        }
    }while(this->bestSolution.path[0] != 0);

    for(int i = 0; i < this->bestSolution.path.size(); i++){ // wypisanie sekwencji
        if(i == this->bestSolution.path.size() - 1)
            std::cout << this->bestSolution.path[i];
        else
            std::cout << this->bestSolution.path[i] << " - ";
    }
    std::cout << " - 0" << std::endl;
    std::cout << std::endl;

    std::cout << "Value: "; // wynikowa droga
    std::cout << this->bestSolution.pathLength;
    std::cout << std::endl << std::endl;

    double period = this->algDuration/16;

    if(this->algDuration >= period * 15){ // wypisanie częściowych rozwiązań
        std::cout << "Solutions after period of time: " << std::endl;
        for(int i = 1; i <= 15; i++){
            std::cout << "Time [" << i*period << "s]: " << partsolutionvalues[i - 1] << std::endl;
            file << partsolutionvalues[i - 1] << std::endl;
        }
        file << std::endl;
    }
    file.close();
}