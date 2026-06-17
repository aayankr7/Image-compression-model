#pragma once
#include "DCdelta.h"

using namespace std;

struct RLEpair
{
    uint8_t runLength;
    int16_t value;
};

vector<vector<RLEpair>> ACshred (vector<array<int16_t,64>>&x)
{
    vector<vector<RLEpair>> y(x.size());

    for(uint32_t i = 0; i<x.size(); i++)
    {   
        int zeroSkip = 0;
        struct RLEpair first;
        first.runLength=0;
        first.value = x[i][0];
        y[i].push_back(first);
        for(int j =1; j<64; j++)
        {   
            if(x[i][j] != 0)
            {
                struct RLEpair temp;
                temp.value = x[i][j];
                temp.runLength = zeroSkip;
                y[i].push_back(temp);
                zeroSkip = 0;
            }else
            {
                int EOBbool = 1;
                for(int k = j; k<64; k++)
                {
                    if(x[i][k] != 0)
                    {
                        EOBbool = 0;
                        break;
                    }
                }

                if(EOBbool)
                {
                    struct RLEpair EOB;
                    EOB.runLength = 0;
                    EOB.value = 0;
                    y[i].push_back(EOB);
                    break;
                }else
                {
                    zeroSkip++;
                    if(zeroSkip == 16)
                    {
                        struct RLEpair ZRL;
                        ZRL.runLength = 15;
                        ZRL.value = 0;
                        y[i].push_back(ZRL);
                        zeroSkip = 0;
                    }
                }
            }
        }
    }

    return y;
}