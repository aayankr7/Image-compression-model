#pragma once
#include"..\\YCbCr-downsampler\\CbCr_downsampler.h"
#include <array>

using namespace std;

vector<array<int8_t,64>> sequence(vector<uint8_t> &x, int w, int h)
{
    vector<array<int8_t,64>> y(w*h/64);
    int m=0, n=0;

    for(int row = 0; row<h ; row = row +8)
    {
        for(int col = 0; col<w; col = col+8)
        {
            for(int i = 0; i<8; i++)
            {
                for(int j = 0; j<8; j++)
                {
                    y[m][n]=x[((row+i)*w)+(col+j)] - 128;
                    n++;
                }
            }
            m++;
            n=0;
        }
    }

    return y;
}
