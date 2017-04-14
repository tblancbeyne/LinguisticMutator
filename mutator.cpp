#include "mutator.hpp"

#include <iostream>
#include <fstream>

namespace mtt
{

    Mutator::Mutator()
    {
        std::random_device rd;
        gen = std::mt19937(rd());

        letter = std::uniform_int_distribution<>{1, 1000000000};

        for (std::size_t i = 0; i < probabilities.size(); ++i)
            for (std::size_t j = 0; j < probabilities.size(); ++j)
                for (std::size_t k = 0; k < probabilities.size(); ++k)
                    for (std::size_t l = 0; l < probabilities.size(); ++l)
                        probabilities[i][j][k][l] = letter(gen)/1000000000.0;
    }

    Mutator::Mutator(const std::string & filename)
    {
        std::ifstream ifile(filename);

        probabilities = {0};

        std::size_t count = 3;
        char c = ifile.get();
        auto prevprevprevindex = c == ' ' ? 26 : static_cast<int>(c) - 97;
        c = ifile.get();
        auto prevprevindex = c == ' ' ? 26 : static_cast<int>(c) - 97;
        c = ifile.get();
        auto previndex = c == ' ' ? 26 : static_cast<int>(c) - 97;

        while (ifile.get(c)) {
            count++;
            auto index = c == ' ' ? 26 : static_cast<int>(c) - 97;
            probabilities[prevprevprevindex][prevprevindex][previndex][index]++;
            prevprevprevindex = prevprevindex;
            prevprevindex = previndex;
            previndex = index;
        }

        for (std::size_t i = 0; i < probabilities.size(); ++i)
            for (std::size_t j = 0; j < probabilities.size(); ++j)
                for (std::size_t k = 0; k < probabilities.size(); ++k)
                    for (std::size_t l = 0; l < probabilities.size(); ++l)
                    {
                        probabilities[i][j][k][l] /= count;
                    }

    }

    std::vector<char> Mutator::createText(const std::size_t n)
    {
        std::vector<char> text;

        int prev,prevprev,prevprevprev;

        // First char
        float currLetter = letter(gen)/1000000000.0;
        std::array<float,27> cumuledProbabilities{0};
        for (std::size_t i = 0; i < probabilities.size(); i++)
        {
            float proba = 0;
            for (std::size_t j = 0; j < probabilities.size(); j++)
            {
                for (std::size_t k = 0; k < probabilities.size(); k++)
                {
                    for (std::size_t l = 0; l < probabilities.size(); l++)
                        proba += probabilities[i][j][k][l];
                }
            }
            cumuledProbabilities[i] = proba;
        }
        for (std::size_t i = 1; i < cumuledProbabilities.size(); i++)
        {
            cumuledProbabilities[i] += cumuledProbabilities[i-1];
        }
        for (std::size_t i = 0; i < cumuledProbabilities.size(); i++)
            cumuledProbabilities[i] /= cumuledProbabilities.at(cumuledProbabilities.size()-1);
        for (std::size_t i = 0; i < cumuledProbabilities.size(); i++)
        {
            if (cumuledProbabilities[i] > currLetter)
            {
                text.push_back(i);
                prev = i;
                break;
            }
        }

        // Second char
        currLetter = letter(gen)/1000000000.0;
        cumuledProbabilities = {0};
        for (std::size_t i = 0; i < probabilities.size(); i++)
        {
            float proba = 0;
            for (std::size_t j = 0; j < probabilities.size(); j++)
            {
                for (std::size_t k = 0; k < probabilities.size(); k++)
                {
                    proba += probabilities[prev][i][j][k];
                }
            }
            cumuledProbabilities[i] = proba;
        }
        for (std::size_t i = 1; i < cumuledProbabilities.size(); i++)
            cumuledProbabilities[i] += cumuledProbabilities[i-1];
        for (std::size_t i = 0; i < cumuledProbabilities.size(); i++)
            cumuledProbabilities[i] /= cumuledProbabilities.at(cumuledProbabilities.size()-1);
        for (std::size_t i = 0; i < cumuledProbabilities.size(); i++)
        {
            if (cumuledProbabilities[i] > currLetter)
            {
                text.push_back(i);
                prevprev = i;
                break;
            }
        }

        // Third char
        currLetter = letter(gen)/1000000000.0;
        cumuledProbabilities = {0};
        for (std::size_t j = 0; j < probabilities.size(); j++)
        {
            float proba = 0;
            for (std::size_t k = 0; k < probabilities.size(); k++)
            {
                proba += probabilities[prev][prevprev][j][k];
            }
            cumuledProbabilities[j] = proba;
        }
        for (std::size_t i = 1; i < cumuledProbabilities.size(); i++)
            cumuledProbabilities[i] += cumuledProbabilities[i-1];
        for (std::size_t i = 0; i < cumuledProbabilities.size(); i++)
            cumuledProbabilities[i] /= cumuledProbabilities.at(cumuledProbabilities.size()-1);
        for (std::size_t i = 0; i < cumuledProbabilities.size(); i++)
        {
            if (cumuledProbabilities[i] > currLetter)
            {
                text.push_back(i);
                prevprevprev = i;
                break;
            }
        }

        // Next chars
        while(text.size() < 10000)
        {
            int prevprevprevprev = 0;
            std::array<float,27> cumuledProbabilities{0};
            currLetter = letter(gen)/1000000000.0;
            cumuledProbabilities[0] = probabilities[prevprevprev][prevprev][prev][0];
            for (std::size_t i = 1; i < probabilities.size(); i++)
                cumuledProbabilities[i] = cumuledProbabilities[i-1] + probabilities[prevprevprev][prevprev][prev][i];
            if (cumuledProbabilities.at(cumuledProbabilities.size()-1) == 0)
            {
                text.pop_back();
                prev = prevprev;
                prevprev = prevprevprev;
                prevprevprev = prevprevprevprev;
                continue;
            }
            for (std::size_t i = 0; i < probabilities.size(); i++)
                cumuledProbabilities[i] /=cumuledProbabilities.at(cumuledProbabilities.size()-1);
            for (std::size_t i = 0; i < cumuledProbabilities.size(); i++)
            {
                if (cumuledProbabilities[i] >= currLetter && (i != 26 || prev != 26))
                {
                    text.push_back(i);
                    prevprevprevprev = prevprevprev;
                    prevprevprev = prevprev;
                    prevprev = prev;
                    prev = i;
                    break;
                }
            }
        }

        return text;
    }
}

std::ostream& operator<<(std::ostream& os, const std::vector<char>& text)
{
    for (std::size_t n = 0; n < text.size(); ++n)
    {
        if (text.at(n) == 26)
        {
            os <<  " ";
        }
        else
        {
            os << (char) (text.at(n) + 97) ;
        }
    }

    return os;
}

