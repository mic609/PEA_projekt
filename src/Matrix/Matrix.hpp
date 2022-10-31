#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<vector>
#include<iostream>

class Matrix{

    private:
    struct Edge{
        int value;
        int row_number;
        int col_number;
    };
    int s; // liczba wierszy/kolumn
    std::vector<std::vector<Edge>> matrix; //graf

    public:
    Matrix(int);
    void readFromFile(std::string);
    void showGraph();
    void removeColumnRow(int, int);
};

#endif