//------------------------------------------------------------------------------------------------------------------------
// UWAGA !
// Implementacja algorytmu nieukończona. Zostały zrobione tylko algorytmy B&B i Brute Force
//------------------------------------------------------------------------------------------------------------------------

// #ifndef DYNAMICPROGRAMMING_HPP
// #define DYNAMICPROGRAMMING_HPP

// #include <Matrix.hpp>
// #include <list>
// #include <stack>
// #include <vector>

// class DynamicProgramming{
//     public:
//     struct Edge{
//         int od_w;
//         int do_w;
//     };
//     struct Memorised{
//         std::list<int> memorisedElements;
//         int memorisedValue;
//     };

//     private:
//     std::list<Edge> finalEdges;
//     int final_result;
//     int beginVertex; // wierzcholek poczatkowy
//     int lastVertex;
//     int vertextokeep; //zmienna pomocnicza
//     std::vector<std::list<Memorised>> allMemorised;

//     int info;

//     public:
//     DynamicProgramming(Matrix);
//     void algorithm(Matrix);
//     int dynamicprog(Matrix, std::list<int>, std::vector<int>);
//     void memorize(std::list<int>, int);
//     int memorizeCheck(std::list<int>);
// };

// #endif