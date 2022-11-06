#include <Matrix.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

Matrix::Matrix(int size){
    s = size;
}

void Matrix::readFromFile(std::string filename){

    // wczytywanie z pliku
    std::fstream file;

    file.open("D:/STUDIA/Semestr_V/PEA/projekt/program/PEA_projekt/tests/" + filename, std::ios::in);

    if(file.good() == false){
        std::cout << "The file could not be opened! " << std::endl;
    }
    else{
        int val;

        file >> s;
        matrix = decltype(matrix)(s, std::vector<Edge>(s));

        for(int i = 0; i < s; i++){
            for(int j = 0; j < s; j++){
                file >> val;
                matrix[i][j].value = val;
                matrix[i][j].row_number = i;
                matrix[i][j].col_number = j;
            }
        }
    }
    
    file.close();
}

void Matrix::showGraph(){
    for(int i = 0; i < s; i++){
        for(int j = 0; j < s; j++){
            std::cout << matrix[i][j].value << " ";
        }
        std::cout << std::endl;
    }
}

void Matrix::removeColumnRow(int rowIndex, int columnIndex){

    for(int i = 0; i < s; i++)
        matrix[rowIndex][i].value = -1;

    for(int i = 0; i < s; i++){
        matrix[i][columnIndex].value = -1;
    }
}

int Matrix::size(){
    return s;
}
