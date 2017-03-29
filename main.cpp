#include <iostream>
#include <fstream>
#include <array>
#include <random>
#include <vector>

int main(int argc, char *argv[])
{
    std::array<std::array<std::array<std::array<float, 27>, 27>, 27>, 27> probabilities{0};
    std::ifstream file{"./rechercheDuTempsPerdu.txt"};
    std::vector<char> text;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> letter(1, 10000);

    std::size_t count = 3;
    char c = file.get();
    auto prevprevprevindex = c == ' ' ? 26 : static_cast<int>(c) - 97;
    c = file.get();
    auto prevprevindex = c == ' ' ? 26 : static_cast<int>(c) - 97;
    c = file.get();
    auto previndex = c == ' ' ? 26 : static_cast<int>(c) - 97;


    while (file.get(c)) {
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
                    probabilities[i][j][k][l] /= count;

    int prev,prevprev,prevprevprev;

    // First char
    float currLetter = letter(gen)/10000.0;
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
    currLetter = letter(gen)/10000.0;
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
        currLetter = letter(gen)/10000.0;
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
