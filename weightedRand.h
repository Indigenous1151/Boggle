// weighted random function in separate file, I feel that this
// function belongs separate from the boggle board
// 
#ifndef WEIGHTED_RAND_H
#define WEIGHTED_RAND_H

#include <vector>
#include <cstdlib>

inline char weightedRand(const std::vector<int>& frequencyTable, const int sum)
{
    int randNum = rand() % sum + 1;
    
    for (int i = 0; i < frequencyTable.size(); i++)
    {
        randNum -= frequencyTable[i];
        if (randNum <= 0)
            return i + 'a';
    }

    return '0'; // Shouldn't happen if input is valid
}

#endif