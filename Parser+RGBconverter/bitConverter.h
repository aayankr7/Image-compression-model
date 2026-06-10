#pragma once
#include <iostream>
#include<fstream>
#include<vector>
using namespace std;

vector<uint16_t> refinedPixels(vector<char> &x)
{
    vector<uint16_t> y((x.size())/2);
    uint32_t j = 0;

    for(uint32_t i = 0; i < y.size(); i++)
    {
        y[i] = (unsigned char)x[j+1]<<8|(unsigned char)x[j];
        j = j+2;
    }

    return y;
}