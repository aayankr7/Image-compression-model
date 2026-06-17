#pragma once
#include"..\\YCbCr-downsampler\\CbCr_downsampler.h"
#include <array>

using namespace std;

vector<array<int8_t,64>> sequence(vector<uint8_t> &x, int w, int h)
{   
    int pw = w;
    int ph = h;
    w = (w+7)/8;
    h = (h+7)/8;
    vector<array<int8_t,64>> y(w*h);
    int m=0, n=0;

    for(int row = 0; row<h ; row++)
    {
        for(int col = 0; col<w; col++)
        {
            for(int i = 0; i<8; i++)
            {
                for(int j = 0; j<8; j++)
                {
                    int safe_row = min(row*8 + i, ph - 1);
                    int safe_col = min(col*8 + j, pw - 1);
                    
                    y[m][n] = x[(safe_row * pw) + safe_col] - 128;
                    n++;
                }
            }
            m++;
            n=0;
        }
    }

    return y;
}
