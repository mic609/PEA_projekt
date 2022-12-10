#ifndef SIMANN_HPP
#define SIMANN_HPP

#include <Matrix.hpp>
#include <vector>

class SimAnn{

    private:
    std::vector<int> solution; // biezace rozwiazanie (wektor wierzcholkow)
    std::vector<int> partsolutionvalues; // rozwiazania po danym okresie czasu
    static double duration_t; // czas trwania algorytmu
    double temperature; // temperatura
    double temperatureMin; // minimalna dozwolona temperatura
    int currentValue; // bieżąca droga wybranego rozwiązania
    static std::string neighbourhoodType; // typ sasiedztwa
    static double a_constant; // wspolczynnik w geometrycznym schemacie chlodzenia
    static int k_geometric; // liczba, do ktorej ewentuanie ma byc podniesiony wspolczynnik a
    static int paramaterSet; // parametry ustawione / nieustawione
    int najk; // najkrotsza znaleziona wartosc drogi po wszystich iteracjach (nie jest rozwiazaniem koncowym!)

    public:
    SimAnn(int, double, double);
    static void setParameters(double, std::string, double, int, int = 1);
    int calculateCurrentValue(Matrix&, std::vector<int>&); // oblicza droge dla danego rozwiazania
    void calculateCurrentValueForThis(Matrix&);
    void algorithm(Matrix&);
    std::vector<int> neighbourhood();
    void cooling();
    void showResult();

};

#endif