#include <Start.hpp>
#include <Matrix.hpp>
#include <iostream>

void Start::welcomeMessage(){

    int user_input;

    std::cout << "Travelling Salesman Problem" << std::endl;
    std::cout << "Choose algorithm: " << std::endl;
    std::cout << "1. Branch and Bound (DFS)" << std::endl;
    std::cout << "2. Branch and Bound (BFS)" << std::endl;
    std::cout << "3. Dynamic Programming" << std::endl;
    std::cout << "4. Brute Force" << std::endl;
    std::cout << "Input: ";
    std::cin >> user_input;

    Start::chooseAlgorithm(user_input);
}

void Start::chooseAlgorithm(int user_inp){

    Matrix matrix(3);

    switch(user_inp){
        case 1:
        {
            std::cout << "Branch and Bound (DFS)" << std::endl;
            std::cout << "1. Choose a file" << std::endl;
            std::cout << "2. Generate random matrix" << std::endl;
            std::cout << "Input: ";
            std::cin >> user_inp;
            break;
        }
        case 2:
        {
            std::cout << "Branch and Bound (BFS)" << std::endl;
            std::cout << "1. Choose a file" << std::endl;
            std::cout << "2. Generate random matrix" << std::endl;
            std::cout << "Input: ";
            std::cin >> user_inp;

            if(user_inp == 1){
                std::string filename;
                std::cout << "Type file name: ";
                std::cin >> filename;
                matrix.readFromFile(filename);
            }

            break;
        }
        case 3:
        {
            std::cout << "Dynamic Programming" << std::endl;
            std::cout << "1. Choose a file" << std::endl;
            std::cout << "2. Generate random matrix" << std::endl;
            std::cout << "Input: ";
            std::cin >> user_inp;
            break;
        }
        case 4:
        {
            std::cout << "Brute Force" << std::endl;
            std::cout << "1. Choose a file" << std::endl;
            std::cout << "2. Generate random matrix" << std::endl;
            std::cout << "Input: ";
            std::cin >> user_inp;
            break;
        }
    }
}
