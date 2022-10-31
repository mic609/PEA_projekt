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

    file.open("../../PEA_projekt/tests/" + filename, std::ios::in);

    if(file.good() == false){
        std::cout << "The file could not be opened! " << std::endl;
    }
    else{
        int value;

        file >> s;
        matrix = decltype(matrix)(s, std::vector<int>(s));

        for(int i = 0; i < s; i++){
            for(int j = 0; j < s; j++){
                file >> value;
                matrix[i][j] = value;
            }
        }
    }
    
    file.close();
}

void Matrix::showGraph(){
    for(int i = 0; i < s; i++){
        for(int j = 0; j < s; j++){
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Matrix::removeColumnRow(int rowIndex, int columnIndex){
    matrix.erase( std::next( std::begin( matrix ), rowIndex ) );

    std::for_each(matrix.begin(), matrix.end(), [&](std::vector<int>& row) {
        row.erase(std::next(row.begin(), columnIndex));
    });

    s--;
}
