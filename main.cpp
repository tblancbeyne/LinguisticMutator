#include <iostream>
#include <vector>

#include "mutator.hpp"

int main(int argc, char *argv[])
{
    mtt::Mutator mut{"./rechercheDuTempsPerdu.txt"};
    std::vector<char> text = mut.createText(10000);

    std::cout << text << std::endl;

    return 0;
}
