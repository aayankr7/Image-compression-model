#pragma once
#include "..\\Parser+RGBconverter\\headers.h"
using namespace std;

struct colour2
{
    int Y = 0;
    int Cb = 0;
    int Cr = 0;
};

vector<struct colour2> YCbCrData(vector<struct colour>&x)
{
    vector<struct colour2> y(x.size());
    for(uint32_t i = 0; i<x.size(); i++)
    {
        y[i].Y = (0.299*x[i].R) + (0.587*x[i].G) + (0.114*x[i].B);
        y[i].Cb = (-0.1687*x[i].R) + (-0.3313*x[i].G) + (0.5*x[i].B) + 128;
        y[i].Cr = (0.5*x[i].R) + (-0.4187*x[i].G) + (-0.0813*x[i].B) + 128;

        if(y[i].Y>255)
        {
            y[i].Y=255;
        }else if(y[i].Y<0)
        {
            y[i].Y=0;
        }
        if(y[i].Cb>255)
        {
            y[i].Cb=255;
        }else if(y[i].Cb<0)
        {
            y[i].Cb=0;
        }
        if(y[i].Cr>255)
        {
            y[i].Cr=255;
        }else if(y[i].Cr<0)
        {
            y[i].Cr=0;
        }
    }
    return y;
}
