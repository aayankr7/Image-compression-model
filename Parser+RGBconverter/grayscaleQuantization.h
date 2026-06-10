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
        int16_t rawpixel;
        rawpixel = (int16_t)x[i] - (int16_t)512;
        x[i] = rawpixel;
        if(rawpixel<0)
        {
            x[i] = 0;
        }
        uint16_t lossyPixel = (((uint32_t)x[i]*255) + 7935)/15871;
        y[i] = lossyPixel; 
    }

    return y;
}