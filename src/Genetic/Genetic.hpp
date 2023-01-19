#ifndef GENETIC_HPP
#define GENETIC_HPP

#include <vector>
#include <iostream>
#include <Matrix.hpp>

class Genetic{
    private:

    struct Chromosome{ // pojedynczy osobnik w populacji
        std::vector<int> path; // wyznaczona sciezka
        int pathLength; // dlugosc sciezki
    };
    std::vector<Chromosome> population; // populacja
    std::vector<Chromosome> populationGeneric; // populacja po selekcji metoda ruletki
    std::vector<Chromosome> populationElite;  // 10% najlepszej populacji z population_generic
    std::vector<int> partsolutionvalues;
    static double crossProbability; // prawdopodobienstwo krzyzowania
    static double mutationProbability; // prawdopodobienstwo mutacji
    static std::string mutationType; // typ mutacji
    static int populationSize; // wielkosc populacji
    static double algDuration; // kryterium stopu
    Chromosome bestSolution; // najlepszy osobnik
    int generationCounter;

    public:
    static int paramaterSet;

    public:

    Genetic();
    static void setParameters(double, double, int, std::string, double, int = 1);
    calculateCurrentValue(Matrix&, std::vector<int>&);
    void algorithm(Matrix&);
    void populationInit(Matrix&);
    void rouletteSelection();
    void davisCrossover(Matrix&);
    void mutate(Matrix&);
    void elite();
    void showResult();
};

#endif