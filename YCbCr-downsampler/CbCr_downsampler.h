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
        int nextRow = min(row+1, height-1);
        for(int col = 0; col<width; col=col+2)
        {
            int nextCol = min(col + 1, width-1);
            y[i] = ((x[(row*width)+col].Cb + x[((nextRow)*width)+col].Cb+ x[(row*width)+nextCol].Cb + x[((nextRow)*width)+nextCol].Cb)/4) +0.5f;
            i++;
        }
    }

    return y;
}

vector<uint8_t> ChromaRed (vector<struct colour2> &x, int width, int height)
{
    vector<uint8_t> y(x.size()/4);
    int i = 0;
    for(int row = 0; row<height; row=row+2)
    {   
        int nextRow = min(row+1, height-1);
        for(int col = 0; col<width; col=col+2)
        {   
            int nextCol = min(col + 1, width-1);
            y[i] = ((x[(row*width)+col].Cr + x[((nextRow)*width)+col].Cr+ x[(row*width)+nextCol].Cr + x[((nextRow)*width)+nextCol].Cr)/4) +0.5f;
            i++;
        }
    }
    
    return y;
}