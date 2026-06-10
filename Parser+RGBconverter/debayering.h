#pragma once
#include"grayscaleQuantization.h"

struct colour
{
    uint8_t R = 0;
    uint8_t G = 0;
    uint8_t B = 0;
};

vector<struct colour> colouredPixels(vector<char> &x, int width, int height, vector<int> bayerPattern)
{   
    vector<struct colour> rawColour(x.size());
    for(uint32_t i = 0; i<x.size(); i++)
    {
        int col = i%width;
        int row = i/width;
        int patternIndex = ((row%2)*2) + (col%2);
        int color = bayerPattern[patternIndex];
        if(color == 0)
        {
            rawColour[i].R = x[i];
        }else if(color == 1)
        {
            rawColour[i].G = x[i];
        }else
        {
            rawColour[i].B = x[i];
        }
    }
    vector<struct colour> guessedColour(x.size());

    for(uint32_t i = 0; i<x.size(); i++)
    {
        int col = i%width;
        int row = i/width;
        int patternIndex = ((row%2)*2) + (col%2);
        int color = bayerPattern[patternIndex];
        if(col != 0 && row != 0 && col != width-1 && row != height-1)
        {
            if(color == 0)
            {
                guessedColour[i].R = rawColour[i].R;
                guessedColour[i].G = (rawColour[i+1].G + rawColour[i-1].G + rawColour[((row-1)*width)+col].G + rawColour[((row+1)*width)+col].G)/4;
                guessedColour[i].B = (rawColour[((row-1)*width)+col-1].B + rawColour[((row-1)*width)+col+1].B + rawColour[((row+1)*width)+col-1].B + rawColour[((row+1)*width)+col+1].B)/4;
            }else if(color == 1)
            {
                guessedColour[i].G = rawColour[i].G;
                guessedColour[i].R = (rawColour[i+1].R + rawColour[i-1].R + rawColour[((row-1)*width)+col].R + rawColour[((row+1)*width)+col].R)/2;
                guessedColour[i].B = (rawColour[i+1].B + rawColour[i-1].B + rawColour[((row-1)*width)+col].B + rawColour[((row+1)*width)+col].B)/2;
            }else if(color == 2)
            {
                guessedColour[i].B = rawColour[i].B;
                guessedColour[i].G = (rawColour[i+1].G + rawColour[i-1].G + rawColour[((row-1)*width)+col].G + rawColour[((row+1)*width)+col].G)/4;
                guessedColour[i].R = (rawColour[((row-1)*width)+col-1].R + rawColour[((row-1)*width)+col+1].R + rawColour[((row+1)*width)+col-1].R + rawColour[((row+1)*width)+col+1].R)/4;
            }
        }else
        {
            guessedColour[i].R = rawColour[i].R;
            guessedColour[i].G = rawColour[i].G;
            guessedColour[i].B = rawColour[i].B;
        }
    }

    return guessedColour;
}