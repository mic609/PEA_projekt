#include "Start.hpp"
#include <random>

int main(){
    srand(time(NULL));
    Start program;
    program.welcomeMessage();
    return 0;
}