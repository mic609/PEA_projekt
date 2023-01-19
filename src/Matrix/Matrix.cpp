#include <Matrix.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <random>

Matrix::Matrix(int size){
    s = size; // Ustawiamy wielkość macierzy
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda, wczytująca macierz z pliku
//--------------------------------------------------------------------------------------------------------------------------
int Matrix::readFromFile(std::string filename){

    std::fstream file;

    file.open("../../PEA_projekt/tests/" + filename, std::ios::in);

    if(file.good() == false){
        std::cout << "The file could not be opened! " << std::endl;
        return 0;
    }
    else{
        int val;

        file >> s;
        matrix = decltype(matrix)(s, std::vector<Edge>(s));

        // Poniżej przypisujemy wartości z wczytanego pliku, do naszej macierzy 
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
    return 1;
}

//--------------------------------------------------------------------------------------------------------------------------
// Metoda generuje losową macierz
//--------------------------------------------------------------------------------------------------------------------------
void Matrix::generateRandom(int size){
    
    if(size > 0)
        s = size;
    else{ // Wielkość macierzy jest generowana losowo z przedziału <min, max> jeśli użytkownik nie podał jej wielkości
        int max = 20;
        int min = 3;
        int range = max - min + 1;
        s = rand() % range + min;
    }

    matrix = decltype(matrix)(s, std::vector<Edge>(s));

    // Poniżej przypisujemy losowe wartości do macierzy z wybranego przydziału
    for(int i = 0; i < s; i++){
        for(int j = 0; j < s; j++){
            if(i == j){
                matrix[i][j].value = 0;
                matrix[i][j].row_number = i;
                matrix[i][j].col_number = j;
            }
            else{
                matrix[i][j].value = rand()%1000+1; // widoczny przedział
                matrix[i][j].row_number = i;
                matrix[i][j].col_number = j;
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------
// Funkcja wyświetla macierz
//--------------------------------------------------------------------------------------------------------------------------
void Matrix::showGraph(){
    for(int i = 0; i < s; i++){
        for(int j = 0; j < s; j++){
            std::cout << matrix[i][j].value << " ";
        }
        std::cout << std::endl;
    }
}

//--------------------------------------------------------------------------------------------------------------------------
// Funkcja usuwa kolumnę o indeksie columnIndex i wiersz o indeksie rowIndex
//--------------------------------------------------------------------------------------------------------------------------
void Matrix::removeColumnRow(int rowIndex, int columnIndex){

    for(int i = 0; i < s; i++)
        matrix[rowIndex][i].value = -1;

    for(int i = 0; i < s; i++){
        matrix[i][columnIndex].value = -1;
    }
}

//--------------------------------------------------------------------------------------------------------------------------
// Funkcja zwraca wymiar macierzy
//--------------------------------------------------------------------------------------------------------------------------
int Matrix::size(){
    return s;
}
