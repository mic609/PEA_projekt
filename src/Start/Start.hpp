#ifndef START_HPP
#define START_HPP

#include <iostream>

class Start{
    public:
    bool checkInteger(std::string);
    void welcomeMessage();
    long long int read_QPC();

    private:
    void chooseAlgorithm(std::string);
};

#endif