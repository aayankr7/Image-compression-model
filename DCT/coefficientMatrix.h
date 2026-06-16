#pragma once
#include "sequencer.h"
#include <cmath>
using namespace std;

#define pi 3.1415

vector<array<int16_t,64>> coefficients(vector<array<int8_t,64>>&x)
{
    vector<array<int16_t,64>> y(x.size());
    for(uint32_t i = 0; i<x.size(); i++)
    {   
        int j;
        int k = 0;
        for(int v = 0; v<8; v++)
        {
            for(int u = 0; u<8; u++)
            {   
                float sum = 0;
                float au=1, av=1;
                if(u==0)
                {
                    au = 1/sqrt(float(2));
                }
                if(v==0)
                {
                    av = 1/sqrt(float(2));
                }
                for(j=0; j<64; j++)
                {
                    sum = sum + ((float)x[i][j])*(au)*(av)*cos(((2*(j%8))+1)*u*pi/16)*cos(((2*(j/8))+1)*v*pi/16);
                }  
                y[i][k] = (sum/4) + 0.5f;
                k++; 
            }
        }
    }
    return y;
}