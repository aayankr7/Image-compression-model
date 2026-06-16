#pragma once
#include "..\\DCT\\zigzag.h"

using namespace std;

void dcDelta(vector<array<int16_t,64>>&x)
{   
    int prevValue = x[0][0];
    int currentValue;

    for(uint32_t i = 1; i<x.size(); i++)
    {
        currentValue = x[i][0];
        if((int)x[i][0] - prevValue>127)
        {
            x[i][0] = 127;
        }else if((int)x[i][0] - prevValue<-128)
        {
            x[i][0] = -128;
        }else
        {
            x[i][0] = x[i][0] - prevValue;
        }
        prevValue = currentValue;
    }
}