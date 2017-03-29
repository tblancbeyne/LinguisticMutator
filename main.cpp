#include <iostream>
#include <fstream>
#include <array>
#include <random>
#include <vector>

int main(int argc, char *argv[])
{

    std::array<std::array<std::array<float, 27>, 27>, 27> probabilities{0};
    std::ifstream file{"./rechercheDuTempsPerdu.txt"};
    std::vector<char> text;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> letter(1, 10000);

    std::size_t count = 2;
    char c = file.get();
    auto prevprevindex = c == ' ' ? 26 : static_cast<int>(c) - 97;
    c = file.get();
    auto previndex = c == ' ' ? 26 : static_cast<int>(c) - 97;

    while (file.get(c)) {
        count++;
        auto index = c == ' ' ? 26 : static_cast<int>(c) - 97;
        probabilities[prevprevindex][previndex][index]++;
        prevprevindex = previndex;
        previndex = index;
    }

    for (std::size_t i = 0; i < probabilities.size(); i++)
        for (std::size_t j = 0; j < probabilities.size(); j++)
            for (std::size_t k = 0; k < probabilities.size(); k++)
                probabilities[i][j][k] /= count;

    //std::cout << probabilities[0][1][2] << std::endl;

    // First char
    int prev,prevprev;
    float currLetter = letter(gen)/10000.0;
    std::array<float,27> cumuledProbabilities{0};
    for (std::size_t i = 0; i < probabilities.size(); i++)
    {
        float proba = 0;
        for (std::size_t j = 0; j < probabilities.size(); j++)
        {
            for (std::size_t k = 0; k < probabilities.size(); k++)
            {
                proba += probabilities[i][j][k];
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
            prev = i;
            break;
        }
    }

    // Second char
    currLetter = letter(gen)/10000.0;
    cumuledProbabilities = {0};
    for (std::size_t j = 0; j < probabilities.size(); j++)
    {
        float proba = 0;
        for (std::size_t k = 0; k < probabilities.size(); k++)
        {
            proba += probabilities[prev][j][k];
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
            prevprev = prev;
            prev = i;
            break;
        }
    }

    // Next chars
    while(text.size() < 10000)
    {
        int prevprevprev = {0};
        std::array<float,27> cumuledProbabilities{0};
        currLetter = letter(gen)/10000.0;
        cumuledProbabilities[0] = probabilities[prevprev][prev][0];
        for (std::size_t i = 1; i < probabilities.size(); i++)
            cumuledProbabilities[i] = cumuledProbabilities[i-1] + probabilities[prevprev][prev][i];
        if (cumuledProbabilities.at(cumuledProbabilities.size()-1) == 0)
        {
            text.pop_back();
            prev = prevprev;
            prevprev = prevprevprev;
            continue;
        }
        for (std::size_t i = 0; i < probabilities.size(); i++)
            cumuledProbabilities[i] /=cumuledProbabilities.at(cumuledProbabilities.size()-1);
        for (std::size_t i = 0; i < cumuledProbabilities.size(); i++)
        {
            if (cumuledProbabilities[i] >= currLetter && (i != 26 || prev != 26))
            {
                text.push_back(i);
                prevprevprev = prevprev;
                prevprev = prev;
                prev = i;
                break;
            }
        }
    }

    for (std::size_t n=0; n < text.size(); ++n)
    {
        if (text.at(n) == 26)
        {
            std::cout <<  " ";
        }
        else
        {
            std::cout << (char) (text.at(n) + 97) ;
        }
    }
    std::cout << "\n" << std::endl;
    std::cout << text.size() << std::endl;


    return 0;
}
