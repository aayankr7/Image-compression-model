#pragma once
#include "quantizer.h"

const array<uint8_t, 64> lookUpTable = 
{
    0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};

vector<array<int16_t,64>> zigZagSequence(vector<array<int8_t,64>> &x)
{
    vector<array<int16_t,64>> y(x.size());
    for(uint32_t i = 0; i<x.size(); i++)
    {
        for(int j = 0; j<64; j++)
        {
            y[i][j] = x[i][lookUpTable[j]];
        }
    }

    return y;
}