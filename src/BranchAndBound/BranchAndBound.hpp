#ifndef BRANCHANDBOUND_HPP
#define BRANCHANDBOUND_HPP

#include <Matrix.hpp>
#include <list>

class BranchAndBound{

    public:
    
    struct Edge{
        int od_w;
        int do_w;
    };

    private:

    int lower_bound;
    std::list<Edge> finalEdges;
    int final_result;

    public:

    BranchAndBound();
    static int reduceRowColumn(Matrix&);
    static int bottomLimit(Matrix, int, int);
    bool checkIfConnected(std::list<Edge>);
    void algorithm(Matrix);
    int executionLeft(Matrix&, int, std::list<Edge>, bool = false);
    int executionRight(Matrix&, int, int, int, std::list<Edge>, bool = false);
    void showResult();
};

#endif
