#include <iostream>
#include <vector>

#include "mutator.hpp"

int main(int argc, char *argv[])
{
    mtt::Mutator mut;
    if (argc >= 2)
    {
        mut.addText(argv[1]);
    }
    else
    {
        mut.addText("./rechercheDuTempsPerdu.txt");
    }

    std::vector<char> text;

    if (argc == 3)
    {
        text = mut.createText(atoi(argv[2]));
    }
    else
    {
        text = mut.createText(10000);
    }

    std::cout << text << std::endl;

    return 0;
}
