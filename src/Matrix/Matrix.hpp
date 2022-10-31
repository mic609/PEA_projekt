#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<vector>
#include<iostream>

class Matrix{

    private:
    int s;
    std::vector<std::vector<int>> matrix;

    public:
    Matrix(int);
    void readFromFile(std::string);
    void showGraph();
    void removeColumnRow(int, int);
};

#endif