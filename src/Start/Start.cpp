#include <Start.hpp>
#include <Matrix.hpp>
#include <BranchAndBound.hpp>
#include <BruteForce.hpp>
#include <DynamicProgramming.hpp>
#include <Genetic.hpp>
#include <SimAnn.hpp>
#include <TabuSearch.hpp>
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
//--------------------------------------------------------------------------------------------------------------------------
bool checkInteger(std::string input)
{
    bool isNeg=false;
    int itr=0;
    if(input.size()==0)
        return false;
    if(input[0]=='-')
    {
        isNeg=true;
        itr=1;
    }
 
    for(int i=itr;i<input.size();i++)
    {
        if(!isdigit(input[i]))
            return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------
// Ekran powitalny. Użytkownik wczytuje macierz z pliku lub generuje ją losowo
//--------------------------------------------------------------------------------------------------------------------------
void Start::welcomeMessage(){

    std::string user_input;
    bool input_right;
    while(true){
        std::cout << std::endl << "Travelling Salesman Problem" << std::endl;
        std::cout << "1. Choose a file" << std::endl;
        std::cout << "2. Generate random matrix" << std::endl;
        std::cout << "3. Testing" << std::endl;
        std::cout << "Input: ";
        std::cin >> user_input;

        do{
            input_right = true;
            for (auto &ch : user_input){
                if(ch > '3' || ch < '1'){
                    input_right = false;
                    break;
                }
            }
            
            if(input_right == false){
                std::cout << std::endl << "The chosen input is not correct!" << std::endl;
                std::cout << "Input: ";
                std::cin >> user_input;
                std::cout << std::endl;
            }
        }while(!input_right);
        this->chooseAlgorithm(user_input);
    }
}

//--------------------------------------------------------------------------------------------------------------------------
// Użytkownik wybiera algorytm, który ma rozwiązać problem komiwojażera
//--------------------------------------------------------------------------------------------------------------------------
void Start::chooseAlgorithm(std::string user_inp){

    Matrix matrix(4); // 4 - domyślna wartość

    char user;
    for (auto &ch : user_inp)
        user = ch;

    switch(user){
        case '1':
        {
            std::string filename;
            int statement;
            do{
                std::cout << "Type file name: ";
                std::cin >> filename;
                std::cout << std::endl;
                statement = matrix.readFromFile(filename);
            }while(!statement);

            matrix.showGraph();
            std::cout << std::endl;
            break;
        }
        case '2':
        {   
            int size;
            std::string character;
            bool input_right;

            do{
                std::cout << "Matrix size (0- random size): ";
                std::cin >> character;
                std::cout << std::endl;

                input_right = true;
                for (auto &ch : character){
                    if(ch < '0' || ch > '9'){
                        input_right = false;
                        break;
                    }
                }
                size = stoi(character);
                
                if(input_right == false){
                    std::cout << std::endl << "The chosen input is not correct!" << std::endl;
                }
                else{
                    matrix.generateRandom(size);
                }
            }while(!input_right);

            matrix.showGraph();
            std::cout << std::endl;
            break;
        }
        case '3':
        {
            std::string testinp;
            char user;

            std::cout << std::endl << "What do you want to test?" << std::endl;
            std::cout << "1. Branch And Bound" << std::endl;
            std::cout << "2. Brute Force" << std::endl;

            bool input_right;

            do{
                std::cout << "Input: " << std::endl;
                std::cin >> testinp;
                std::cout << std::endl;

                input_right = true;
                for (auto &ch : testinp){
                    if(ch < '1' || ch > '2'){
                        input_right = false;
                        break;
                    }
                    user = ch;
                }
                
                if(input_right == false){
                    std::cout << std::endl << "The chosen input is not correct!" << std::endl;
                }
            }while(!input_right);

            switch(user){
                case '1':
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
                case '2':{
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

    std::cout << std::endl << "Choose algorithm: " << std::endl;
    std::cout << "1. Branch and Bound" << std::endl;
    std::cout << "2. Brute Force" << std::endl;
    std::cout << "3. Simulated Annealing" << std::endl;
    std::cout << "4. Tabu Search" << std::endl;
    std::cout << "5. Genetic algorithm" << std::endl;

    long long int frequency, start, elapsed;
    QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
    
    while(true){
        bool input_right = true;
        do{
                std::cout << "Input (0- powrot do poprzedniego menu): ";
                std::cin >> user_inp;
                std::cout << std::endl;

                input_right = true;
                for (auto &ch : user_inp){
                    if(ch < '0' || ch > '5'){
                        input_right = false;
                        break;
                    }
                    user = ch;
                }
                
                if(input_right == false){
                    std::cout << std::endl << "The chosen input is not correct!" << std::endl;
                }
        }while(!input_right);
        
        switch(user){
            case '0':{
                return; // powrót do poprzedniego menu
                break;
            }
            case '1':
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
            case '2':
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
            case '3': // simulated annealing
            {
                SimAnn simalg(matrix.size(), 25000, 0);

                int parameterSet;

                do{
                    std::cout << std::endl << "Do you want to set parameters for SA? (1- yes, 0- no): ";
                    std::cin >> parameterSet;

                    double exec_time;
                    std::string neigh_type;
                    double a_const;
                    int k_geom;

                    std::string exec_time_str;
                    std::string a_const_str; 

                    bool inp_incorrect;

                    if(parameterSet == 1){

                        std::string str;

                        do{
                            inp_incorrect = false;
                            std::cout << "Set stopping criterion (execution time in seconds): ";
                            std::cin >> exec_time_str;
                            for (auto &ch : exec_time_str){
                                if(ch < '0' || ch > '9'){
                                    if(ch != '.'){
                                        inp_incorrect = true;
                                    }
                                }
                            }
                            if(!inp_incorrect)
                                exec_time = std::stod(exec_time_str);

                            std::cout << "Set neigbhbourhood type (write: swap, invert): ";
                            std::cin >> neigh_type;
                            if(neigh_type != "swap" && neigh_type != "invert")
                                inp_incorrect = true;

                            std::cout << "Set temperature coefficient a (default- 0.99): ";
                            std::cin >> a_const_str;
                            for (auto &ch : a_const_str){
                                if(ch < '0' || ch > '9'){
                                    if(ch != '.'){
                                        inp_incorrect = true;
                                    }
                                }
                            }
                            if(!inp_incorrect)
                                a_const = std::stod(a_const_str);

                            if(inp_incorrect)
                                std::cout << std::endl <<"Some of the inputs may have invalid format!" << std::endl << std::endl;
                        }while(inp_incorrect);

                        SimAnn::setParameters(exec_time, neigh_type, a_const, k_geom);
                    }
                    
                    if(simalg.paramaterSet == 1){
                        simalg.algorithm(matrix);
                        simalg.showResult();
                    }
                    else{
                        std::cout << "You need to set parameters first before algorithm starts!" <<std::endl;
                    }
                }while(simalg.paramaterSet == 0);


                break;
            }
            case '4': // tabu search
            {
                TabuSearch tabualg;

                int parameterSet;

                do{  
                    std::cout << std::endl << "Do you want to set parameters for TS? (1- yes, 0- no): ";
                    std::cin >> parameterSet;

                    double exec_time;
                    std::string neigh_type;
                    int diver;

                    std::string exec_time_str;
                    std::string diver_str; 

                    bool inp_incorrect;
                    bool exec_algorithm;

                    if(parameterSet == 1){
                        std::string str;

                        do{
                            inp_incorrect = false;
                            std::cout << "Set stopping criterion (execution time in seconds): ";
                            std::cin >> exec_time_str;
                            for (auto &ch : exec_time_str){
                                if(ch < '0' || ch > '9'){
                                    if(ch != '.'){
                                        inp_incorrect = true;
                                    }
                                }
                            }
                            if(!inp_incorrect)
                                exec_time = std::stod(exec_time_str);

                            std::cout << "Set neigbhbourhood type (write: swap, invert): ";
                            std::cin >> neigh_type;
                            if(neigh_type != "swap" && neigh_type != "invert")
                                inp_incorrect = true;

                            std::cout << "Turn on diversifaction? (1- yes, 0- no): ";
                            std::cin >> diver_str;
                            for (auto &ch : diver_str){
                                if(ch < '0' || ch > '1'){
                                    inp_incorrect = true;
                                }
                            }
                            if(!inp_incorrect)
                                diver = std::stoi(diver_str);

                            if(inp_incorrect)
                                std::cout << std::endl <<"Some of the inputs may have invalid format!" << std::endl << std::endl;
                        }while(inp_incorrect);

                        TabuSearch::setParameters(exec_time, neigh_type, diver);

                    }

                    if(tabualg.paramaterSet == 1){
                        tabualg.algorithm(matrix);
                        tabualg.showResult();
                    }
                    else{
                        std::cout << "You need to set parameters first before algorithm starts!" <<std::endl;
                    }
                } while (tabualg.paramaterSet == 0);
                
                break;
            }
            case '5':
            {
                Genetic alg;

                int parameterSet;

                do{  
                    std::cout << std::endl << "Do you want to set parameters for Genetic? (1- yes, 0- no): ";
                    std::cin >> parameterSet;

                    double exec_time;
                    int populationSize;
                    double mutationCoef;
                    double crossoverCoef;
                    std::string mutationType; 

                    std::string exec_time_str;
                    std::string population_str;
                    std::string mut_str;
                    std::string cross_str; 

                    bool inp_incorrect;
                    bool exec_algorithm;

                    if(parameterSet == 1){
                        std::string str;

                        do{
                            inp_incorrect = false;
                            std::cout << "Set stopping criterion (execution time in seconds): ";
                            std::cin >> exec_time_str;
                            for (auto &ch : exec_time_str){
                                if(ch < '0' || ch > '9'){
                                    if(ch != '.'){
                                        inp_incorrect = true;
                                    }
                                }
                            }
                            if(!inp_incorrect)
                                exec_time = std::stod(exec_time_str);

                            std::cout << "Set population size: ";
                            std::cin >> population_str;
                            for (auto &ch : population_str){
                                if(ch < '0' || ch > '9'){
                                    inp_incorrect = true;
                                }
                            }
                            if(!inp_incorrect)
                                populationSize = std::stod(population_str);

                            std::cout << "Set mutation coefficient (recommended- 0.01): ";
                            std::cin >> mut_str;
                            for (auto &ch :mut_str){
                                if(ch < '0' || ch > '9'){
                                    if(ch != '.'){
                                        inp_incorrect = true;
                                    }
                                }
                            }
                            if(!inp_incorrect)
                                mutationCoef = std::stod(mut_str);

                            std::cout << "Set crossover coefficient (recommended- 0.8): ";
                            std::cin >> cross_str;
                            for (auto &ch :cross_str){
                                if(ch < '0' || ch > '9'){
                                    if(ch != '.'){
                                        inp_incorrect = true;
                                    }
                                }
                            }
                            if(!inp_incorrect)
                                crossoverCoef = std::stod(cross_str);

                            std::cout << "Set mutation type (write: transposition, inversion): ";
                            std::cin >> mutationType;
                            if(mutationType != "inversion" && mutationType != "transposition")
                                inp_incorrect = true;

                            if(inp_incorrect)
                                std::cout << std::endl <<"Some of the inputs may have invalid format!" << std::endl << std::endl;
                        }while(inp_incorrect);

                        Genetic::setParameters(crossoverCoef, mutationCoef, populationSize, mutationType, exec_time);

                    }

                    if(alg.paramaterSet == 1){
                        alg.algorithm(matrix);
                        alg.showResult();
                    }
                    else{
                        std::cout << "You need to set parameters first before algorithm starts!" <<std::endl;
                    }
                } while (alg.paramaterSet == 0);
            }
        }
    }
}

