#pragma once
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include"formatChecker.h"
#include"scavenger.h"
#include"bitConverter.h"

vector <char> downscaledPixels(vector<uint16_t> &x)
{
    vector<char> y(x.size());

    for(uint32_t i= 0; i<x.size(); i++)
    {
        uint16_t lossyPixel = ((x[i]*255) + 8191)/16383;
        y[i] = lossyPixel; 
    }

    return y;
}