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

    // wypisywanie wyniku
    for(int i = 0; i < s; i++){
        for(int j = 0; j < s; j++){
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    // USUWANIE WIERSZA
    matrix.erase( std::next( std::begin( matrix ), 1 ) );

    // wypisywanie wyniku
    for(int i = 0; i < (s-1); i++){
        for(int j = 0; j < s; j++){
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // USUWANIE KOLUMNY
    int columnIndex = 1;
    std::for_each(matrix.begin(), matrix.end(), [&](std::vector<int>& row) {
        row.erase(std::next(row.begin(), columnIndex));
    });

    std::cout << std::endl;

    // wypisywanie wyniku
    for(int i = 0; i < (s-1); i++){
        for(int j = 0; j < (s-1); j++){
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}