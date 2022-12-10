#include <Start.hpp>
#include <Matrix.hpp>
#include <BranchAndBound.hpp>
#include <BruteForce.hpp>
#include <DynamicProgramming.hpp>
#include <SimAnn.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <windows.h>

//--------------------------------------------------------------------------------------------------------------------------
// Funkcja do pomiaru czasu
//--------------------------------------------------------------------------------------------------------------------------

long long int Start::read_QPC(){
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return((long long int)count.QuadPart);
}

//--------------------------------------------------------------------------------------------------------------------------
// Ekran powitalny. Użytkownik wczytuje macierz z pliku lub generuje ją losowo
//--------------------------------------------------------------------------------------------------------------------------
void Start::welcomeMessage(){

    int user_input;
    while(true){
        std::cout << std::endl << "Travelling Salesman Problem" << std::endl;
        std::cout << "1. Choose a file" << std::endl;
        std::cout << "2. Generate random matrix" << std::endl;
        std::cout << "3. Testing" << std::endl;
        std::cout << "Input: ";
        std::cin >> user_input;

        while(user_input > 3 || user_input < 1){
            std::cout << std::endl << "The chosen number is not correct. Please type '1' or '2'" << std::endl;
            std::cout << "Input: ";
            std::cin >> user_input;
        }
        this->chooseAlgorithm(user_input);
    }
}

//--------------------------------------------------------------------------------------------------------------------------
// Użytkownik wybiera algorytm, który ma rozwiązać problem komiwojażera
//--------------------------------------------------------------------------------------------------------------------------
void Start::chooseAlgorithm(int user_inp){

    Matrix matrix(4); // 4 - domyślna wartość

    switch(user_inp){
        case 1:
        {
            std::string filename;
            std::cout << "Type file name: ";
            std::cin >> filename;
            std::cout << std::endl;
            matrix.readFromFile(filename);
            matrix.showGraph();
            std::cout << std::endl;
            break;
        }
        case 2:
        {   
            int size;
            std::cout << "Matrix size (0- random size): ";
            std::cin >> size;
            std::cout << std::endl;
            matrix.generateRandom(size);
            matrix.showGraph();
            std::cout << std::endl;
            break;
        }
        case 3:
        {
            int testinp;
            std::cout << std::endl << "What do you want to test?" << std::endl;
            std::cout << "1. Branch And Bound" << std::endl;
            std::cout << "2. Brute Force" << std::endl;
            std::cout << "Input: " << std::endl;
            std::cin >> testinp;
            std::cout << std::endl;
            switch(testinp){
                case 1:
                {
                    // --------------------------------------------------------------------------------------
                    // Branch And Bound
                    // --------------------------------------------------------------------------------------

                    BranchAndBound alg;

                    int testedValue;
                    std::cout << "Tested Value: ";
                    std::cin >> testedValue;

                    long long int frequency, start, elapsed; 
                    float avarageTime, sum_s = 0, sum_ms = 0, sum_us = 0;
                    QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
                    for(int i = 0; i < 100; i++){
                        matrix.generateRandom(testedValue);
                        start = read_QPC();
                        alg.algorithm(matrix); // uruchamiamy algorytm
                        elapsed = read_QPC() - start;
                        std::cout << std::endl << "The number of test: " << i + 1 << std::endl;
                        std::cout << "Time [s] = " << std::fixed << std::setprecision(3) << (float)elapsed /
                        frequency << std::endl;
                        std::cout << "Time [ms] = " << std::setprecision(0) << (1000.0 * elapsed) /
                        frequency << std::endl;
                        std::cout << "Time [us] = " << std::setprecision(0) << (1000000.0 * elapsed) /
                        frequency << std::endl << std::endl;

                        sum_s += (float)elapsed / frequency;
                        sum_ms += (1000.0 * elapsed) /frequency;
                        sum_us += (1000000.0 * elapsed) / frequency;
                    }

                    std::cout << std::endl << std::endl << "Avarage value: " << std::endl;
                    std::cout << "Time [s] = " << std::fixed << std::setprecision(3) << sum_s / 100 << std::endl;
                    std::cout << "Time [ms] = " << std::setprecision(0) << sum_ms / 100 << std::endl;
                    std::cout << "Time [us] = " << std::setprecision(0) << sum_us / 100 << std::endl << std::endl;
                    break;
                }
                case 2:{
                    // --------------------------------------------------------------------------------------
                    // Brute Force
                    // --------------------------------------------------------------------------------------
                    BruteForce alg;

                    int testedValue;
                    std::cout << "Tested Value: ";
                    std::cin >> testedValue;

                    long long int frequency, start, elapsed; 
                    float avarageTime, sum_s = 0, sum_ms = 0, sum_us = 0;
                    QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
                    for(int i = 0; i < 100; i++){
                        matrix.generateRandom(testedValue);
                        start = read_QPC();
                        alg.algorithm(matrix); // uruchamiamy algorytm
                        elapsed = read_QPC() - start;
                        std::cout << std::endl << "The number of test: " << i + 1 << std::endl;
                        std::cout << "Time [s] = " << std::fixed << std::setprecision(3) << (float)elapsed /
                        frequency << std::endl;
                        std::cout << "Time [ms] = " << std::setprecision(0) << (1000.0 * elapsed) /
                        frequency << std::endl;
                        std::cout << "Time [us] = " << std::setprecision(0) << (1000000.0 * elapsed) /
                        frequency << std::endl << std::endl;

                        sum_s += (float)elapsed / frequency;
                        sum_ms += (1000.0 * elapsed) /frequency;
                        sum_us += (1000000.0 * elapsed) / frequency;
                    }

                    std::cout << std::endl << std::endl << "Avarage value: " << std::endl;
                    std::cout << "Time [s] = " << std::fixed << std::setprecision(3) << sum_s / 100 << std::endl;
                    std::cout << "Time [ms] = " << std::setprecision(0) << sum_ms / 100 << std::endl;
                    std::cout << "Time [us] = " << std::setprecision(0) << sum_us / 100 << std::endl << std::endl;
                    break;
                }
            }
            break;
        }
    }

    std::cout << "Choose algorithm: " << std::endl;
    std::cout << "1. Branch and Bound" << std::endl;
    std::cout << "2. Brute Force" << std::endl;
    std::cout << "3. Simulated Annealing" << std::endl;

    long long int frequency, start, elapsed;
    QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
    
    while(true){
        std::cout << "Input (0- powrot do poprzedniego menu): ";
        std::cin >> user_inp;
        switch(user_inp){
            case 0:{
                return; // powrót do poprzedniego menu
                break;
            }
            case 1:
            {
                BranchAndBound alg;
                start = read_QPC();
                alg.algorithm(matrix); // uruchamiamy algorytm
                elapsed = read_QPC() - start;

                alg.showResult(); // wyświetlamy wynik
                std::cout<<std::endl<<std::endl;

                std::cout << "Time [s] = " << std::fixed << std::setprecision(3) << (float)elapsed /
                frequency << std::endl;
                std::cout << "Time [ms] = " << std::setprecision(0) << (1000.0 * elapsed) /
                frequency << std::endl;
                std::cout << "Time [us] = " << std::setprecision(0) << (1000000.0 * elapsed) /
                frequency << std::endl << std::endl;
                break;

            }
            case 2:
            {
                BruteForce alg;

                start = read_QPC();
                alg.algorithm(matrix); // uruchamiamy algorytm
                elapsed = read_QPC() - start;

                alg.showResult(); // wyświetlamy wynik
                std::cout<<std::endl<<std::endl;

                std::cout << "Time [s] = " << std::fixed << std::setprecision(3) << (float)elapsed /
                frequency << std::endl;
                std::cout << "Time [ms] = " << std::setprecision(0) << (1000.0 * elapsed) /
                frequency << std::endl;
                std::cout << "Time [us] = " << std::setprecision(0) << (1000000.0 * elapsed) /
                frequency << std::endl << std::endl;
                break;
            }
            case 3:
            {
                SimAnn simalg(matrix.size(), 25000, 0);

                int parameterSet = true;

                std::cout << std::endl << "Do you want to set parameters for SA? (1- yes, 0- no): ";
                std::cin >> parameterSet;

                double exec_time;
                std::string neigh_type;
                double a_const;
                int k_geom;

                if(parameterSet == 1){

                    std::cout << "Set stopping criterion (execution time in seconds): ";
                    std::cin >> exec_time;
                    std::cout << "Set neigbhbourhood type (write: swap, invert): ";
                    std::cin >> neigh_type;
                    std::cout << "Set temperature coeffiecient a: ";
                    std::cin >> a_const;
                    SimAnn::setParameters(exec_time, neigh_type, a_const, k_geom);
                }
                
                simalg.algorithm(matrix);
                simalg.showResult();
                break;
            }
        }
    }
}

