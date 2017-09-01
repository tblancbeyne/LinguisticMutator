#ifndef MTT_MUTATOR_HPP
#define MTT_MUTATOR_HPP

#include <array>
#include <random>

namespace mtt
{

class Mutator {
    public:
        Mutator();
        Mutator(const std::string & filename);
        Mutator(std::string & expr);
        void addText(const std::string & filename);
        std::vector<char> createText(const std::size_t n);
        void transformText(std::vector<char> & text);

    private:
        std::mt19937 gen;
        std::uniform_int_distribution<> letter;
        std::array<std::array<std::array<std::array<float, 27>, 27>, 27>, 27> probabilities;
        void _setProbas(const std::string & filename);
};

}

std::ostream& operator<<(std::ostream& os, const std::vector<char>& obj);

#endif // MTT_MUTATOR_HPP
