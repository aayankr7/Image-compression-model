#pragma once
#include "..\\Parser+RGBconverter\\headers.h"
#include "YCbCr_converter.h"
using namespace std;

vector<uint8_t> Luma (vector<struct colour2>&x, int width, int height)
{
    vector<uint8_t> y(x.size());

    for(uint32_t i = 0; i<x.size(); i++)
    {
        y[i] = x[i].Y;
    }

    return y;
}

vector<uint8_t> ChromaBlue (vector<struct colour2> &x, int width, int height)
{
    vector<uint8_t> y(x.size()/4);
    int i = 0;
    for(int row = 0; row<height; row=row+2)
    {
        for(int col = 0; col<width; col=col+2)
        {
            y[i] = ((x[(row*width)+col].Cb + x[((row+1)*width)+col].Cb+ x[(row*width)+col+1].Cb + x[((row+1)*width)+col+1].Cb)/4) +0.5f;
            i++;
        }
    }

    return y;
}

vector<uint8_t> ChromaRed (vector<struct colour2> &x, int width, int height)
{
    vector<uint8_t> y(x.size()/4);
    int i = 0;
    for(int row = 0; row<height-2; row=row+2)
    {
        for(int col = 0; col<width-2; col=col+2)
        {
            y[i] = ((x[(row*width)+col].Cr + x[((row+1)*width)+col].Cr+ x[(row*width)+col+1].Cr + x[((row+1)*width)+col+1].Cr)/4) +0.5f;
            i++;
        }
    }
    
    return y;
}