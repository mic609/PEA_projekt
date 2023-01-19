#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<vector>
#include<iostream>

class Matrix{

    public:

    struct Edge{
        int value;
        int row_number;
        int col_number;
    };
    std::vector<std::vector<Edge>> matrix; //graf
    
    private:

    int s; // liczba wierszy/kolumn

    public:
    
    Matrix(int);
    int readFromFile(std::string);
    void generateRandom(int = 0);
    void showGraph();
    void removeColumnRow(int, int);
    int size();
};

#endif