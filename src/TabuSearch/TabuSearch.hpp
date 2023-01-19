#ifndef TABUSEARCH_HPP
#define TABUSEARCH_HPP

#include <Matrix.hpp>
#include <Vector>

class TabuSearch{
    private:
    std::vector<int> solution; // biezace rozwiazanie (wektor wierzcholkow)
    int currentValue; // bieżąca droga wybranego rozwiązania
    std::vector<int> proposedSolution; // potencjalne rozwiazanie (wektor wierzcholkow)
    int proposedValue; // biezaca droga potencjalnego rozwiazania
    std::vector<int> partsolutionvalues; // rozwiazania po danym okresie czasu
    static double duration_t; // czas trwania algorytmu
    static std::string neighbourhoodType; // typ sasiedztwa
    static int diversification; // wlaczenie / wylaczenie dywersyfikacji
    std::vector<std::vector<int>> tabulist; // lista tabu przechowujaca wartosc kadencji
    int tabucount; // liczba elementow na liscie tabu
    int best_from; // najlepsze przejscie w sasiedztwie
    int best_to; // najlepsze przejscie w sasiedztwie

    public:
    static int paramaterSet; // parametry ustawione / nieustawione

    public:
    TabuSearch();
    static void setParameters(double, std::string, int, int = 1);
    int calculateCurrentValue(Matrix&, std::vector<int>&); // oblicza droge dla danego rozwiazania
    void calculateCurrentValueForThis(Matrix&);
    void algorithm(Matrix&);
    std::vector<int> neighbourhoodLook(Matrix&);
    std::vector<int> neighbourhood(int, int);
    void showResult();
};

#endif