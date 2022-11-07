#ifndef BRUTEFORCE_HPP
#define BRUTEFORCE_HPP

#include <Matrix.hpp>
#include <list>

class BruteForce{
    public:
    struct Edge{
        int od_w;
        int do_w;
    };

    private:
    std::list<Edge> finalEdges;
    int final_result;

    public:
    void algorithm(Matrix);
    void showResult();
};

#endif