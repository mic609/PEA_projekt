//------------------------------------------------------------------------------------------------------------------------
// UWAGA !
// Implementacja algorytmu nieukończona. Zostały zrobione tylko algorytmy B&B i Brute Force
//------------------------------------------------------------------------------------------------------------------------

// #include <DynamicProgramming.hpp>
// #include <Matrix.hpp>
// #include <list>
// #include <algorithm>
// #include <vector>
// #include <iterator>
// #include <bits/stdc++.h>

// DynamicProgramming::DynamicProgramming(Matrix matrix){
//     info = 0;
//     allMemorised.resize(matrix.size() - 1);
// }

// //--------------------------------------------------------------------------------------------------------------------------
// void DynamicProgramming::algorithm(Matrix matrix){
//     beginVertex = 0;
//     lastVertex = 0;
//     final_result = 0;

//     std::vector<int> visited(matrix.size());
//     std::list<int> awaiting;

//     for(int i = matrix.size() - 1; i >= 0; i--)
//         awaiting.push_front(i);

//     for(auto v : awaiting)
//         std::cout << v+1 << std::endl;
    
//     std::cout << std::endl;

//     visited[beginVertex] = 1;
//     awaiting.remove(beginVertex);

//     for(auto v : awaiting)
//         std::cout << v+1 << std::endl;
    
//     std::cout << std::endl;

//     final_result = dynamicprog(matrix, awaiting, visited);

//     std::cout << "Wynik: " << final_result << std::endl;
//     std::cout << "Info: " << info << std::endl;

// }

// //--------------------------------------------------------------------------------------------------------------------------
// int DynamicProgramming::dynamicprog(Matrix matrix, std::list<int> awaiting, std::vector<int> visited){
//     int result;

//     if(awaiting.size() == 0){
//         int poppedVertex = vertextokeep;
//         visited[poppedVertex] = 1;
//         result = matrix.matrix[poppedVertex][beginVertex].value;
//         //awaiting.remove(poppedVertex);
//         return result;
//     }
//     else{
//         int results[awaiting.size()];
//         int resultIndex = 0;
//         for(int i = 0; i < matrix.size(); i++){
//             if(visited[i] == 0){
//                 if(lastVertex != i){

//                     if(awaiting.size() <= 1){
//                         vertextokeep = i;
//                         awaiting.remove(i);
//                     }
//                     else
//                         awaiting.remove(i);

//                     //std::cout << "Info: " << info << std::endl;

//                     visited[i] = 1;
//                     int lastVertex_temp = lastVertex;
//                     lastVertex = i;

                    
//                     time_t start, end;
//                     time(&start);
//                     // unsync the I/O of C and C++.
//                     std::ios_base::sync_with_stdio(false);
//                     // Recording end time.
//                     int memorizereturn = memorizeCheck(awaiting);
//                     time(&end);
//                     double time_taken = double(end - start);

//                     std::cout << "time taken: " << time_taken << std::setprecision(5) << std::endl;

//                     if(memorizereturn == -1){ // jesli nie znaleziono wartosci
//                         results[resultIndex] = dynamicprog(matrix, awaiting, visited) + matrix.matrix[lastVertex_temp][i].value;
//                         awaiting.sort();
//                         int value = results[resultIndex] - matrix.matrix[lastVertex_temp][i].value;
//                         if(awaiting.size() == 0){
//                             awaiting.push_front(i);
//                             memorize(awaiting, value);
//                             awaiting.pop_front();
//                         }
//                         else
//                             memorize(awaiting, value);
//                     }
//                     else{
//                         info ++;
//                         results[resultIndex] = memorizereturn + matrix.matrix[lastVertex_temp][i].value;
//                     }

//                     lastVertex = lastVertex_temp;
//                     awaiting.push_front(i);
//                     visited[i] = 0;
//                     resultIndex ++;
//                 }
//             }
//         }
//         result = *std::min_element(results, results + awaiting.size());
//     }
//     return result;

// }

// //--------------------------------------------------------------------------------------------------------------------------
// // zapisuje w pamieci dany rezultat
// //--------------------------------------------------------------------------------------------------------------------------
// void DynamicProgramming::memorize(std::list<int> awaiting, int value){

//     int awaitingSize = awaiting.size();
//     Memorised memorised;

//     std::list<int>::iterator it;
//     for(it = awaiting.begin(); it != awaiting.end(); it++)
//         memorised.memorisedElements.push_front(*it);

//     memorised.memorisedValue = value;
    
//     allMemorised[awaitingSize].push_front(memorised);
// }

// //--------------------------------------------------------------------------------------------------------------------------
// // sprawdzamy pamiec
// //--------------------------------------------------------------------------------------------------------------------------
// int DynamicProgramming::memorizeCheck(std::list<int> awaiting){
//     int checkSuitable = 0;
//     std::list<int>::iterator it;
    
//     for(auto v: allMemorised[awaiting.size()]){

//         it = awaiting.begin();

//         for(auto u : v.memorisedElements){
//             if(*it == u){
//                 checkSuitable++;
//                 it++;
//             }
//         }
//         if(checkSuitable == awaiting.size() && checkSuitable != 0)
//             return v.memorisedValue;
        
//         checkSuitable = 0;
//     }
//     return -1;
// }