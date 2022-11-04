#ifndef BRANCHANDBOUND_HPP
#define BRANCHANDBOUND_HPP

#include <Matrix.hpp>

class BranchAndBound{

    private:
    int lower_bound;
    int result;

    public:
    BranchAndBound(int);
    static int reduceRowColumn(Matrix&); // zwraca wartość redukcji
    static int bottomLimit(Matrix, int, int);
    void algorithm(Matrix);
    void execution(Matrix&, bool = false);
    void showResult();
};

#endif
