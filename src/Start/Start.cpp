#include <Start.hpp>
#include <Matrix.hpp>
#include <BranchAndBound.hpp>
#include <iostream>
#include <sstream>

void Start::welcomeMessage(){

    int user_input;
    std::cout << "Travelling Salesman Problem" << std::endl;
    std::cout << "1. Choose a file" << std::endl;
    std::cout << "2. Generate random matrix" << std::endl;
    std::cout << "Input: ";
    std::cin >> user_input;

    while(user_input > 2 || user_input < 1){
        std::cout << std::endl << "The chosen number is not correct. Please type '1' or '2'" << std::endl;
        std::cout << "Input: ";
        std::cin >> user_input;
    }
    Start::chooseAlgorithm(user_input);
}

void Start::chooseAlgorithm(int user_inp){

    Matrix matrix(4); // 4 - domyślna wartość

    switch(user_inp){
        case 1:
        {
            std::string filename;
            std::cout << "Type file name: ";
            std::cin >> filename;
            matrix.readFromFile(filename);
            matrix.showGraph();
            break;
        }
        case 2:
        {   
            int size;
            std::cout << "Matrix size (0- random size): ";
            std::cin >> size;
            matrix.generateRandom(size);
            matrix.showGraph();
            break;
        }
    }

    std::cout << "Choose algorithm: " << std::endl;
    std::cout << "1. Branch and Bound (DFS)" << std::endl;
    std::cout << "2. Branch and Bound (BFS)" << std::endl;
    std::cout << "3. Dynamic Programming" << std::endl;
    std::cout << "4. Brute Force" << std::endl;
    std::cout << "Input: ";
    std::cin >> user_inp;

    switch(user_inp){
        case 1:
        {
            BranchAndBound alg;
            // uruchamiamy algorytm
            alg.algorithm(matrix);

            int bruteForceinput;
            std::cout << "Uruchomic Brute Force? (0- NIE)" << std::endl;
            std::cout << "Input: ";
            std::cin >> bruteForceinput;
            if(bruteForceinput)
                alg.bruteForce(matrix);

            std::cout<<std::endl<<std::endl;
            break;
        }
    }
}
