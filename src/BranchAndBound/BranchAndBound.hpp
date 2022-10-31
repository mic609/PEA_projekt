#ifndef BRANCHANDBOUND_HPP
#define BRANCHANDBOUND_HPP

#include <Matrix.hpp>

class BranchAndBound{
    public:
    static int reduceRowColumn(Matrix); // zwraca wartość redukcji
    static void algorithm(Matrix);
};

#endif
