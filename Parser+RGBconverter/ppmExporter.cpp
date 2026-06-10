#include<iostream>
#include"scavenger.h"
#include"formatChecker.h"
#include"bitConverter.h"
#include"grayscaleQuantization.h"
#include <vector>
#include <string>
#include <fstream>
#include "bayerPattern.h"
#include "debayering.h"
#include<cmath>
#include<algorithm>

int main()
{
    string filepath = "..\\test\\testImage2.ARW";
    uint32_t stripBytesCount;
    if(check(filepath, &stripBytesCount))
    {
        return 1;
    }

    int width, height;

    vector<char> messyPixelData(stripBytesCount);
    messyPixelData = sensorData(filepath, &width, &height);

    vector<uint16_t> pixelData (stripBytesCount/2);
    pixelData = refinedPixels(messyPixelData);

    vector<char> lossyPixels(stripBytesCount/2);
    lossyPixels = downscaledPixels(pixelData);

    vector<int> x(4);
    x = bayerPattern(filepath); 

    vector<struct colour> colourInfo(lossyPixels.size());
    colourInfo = colouredPixels(lossyPixels, width, height, x);

float redBalance = 2.4f; 
float blueBalance = 1.8f; 
float gammaTarget = 1.0f / 2.2f;

for(size_t i = 0; i < colourInfo.size(); i++) {
    
    float r = colourInfo[i].R;
    float g = colourInfo[i].G;
    float b = colourInfo[i].B;

    r *= redBalance;
    g *= 1.0f;
    b *= blueBalance;

    float finalR = (r * 1.40f) + (g * -0.20f) + (b * -0.20f);
    float finalG = (r * -0.15f) + (g * 1.30f) + (b * -0.15f);
    float finalB = (r * -0.10f) + (g * -0.20f) + (b * 1.30f);

    finalR = max(0.0f, min(finalR, 255.0f));
    finalG = max(0.0f, min(finalG, 255.0f));
    finalB = max(0.0f, min(finalB, 255.0f));

    finalR = 255.0f * pow(finalR / 255.0f, gammaTarget);
    finalG = 255.0f * pow(finalG / 255.0f, gammaTarget);
    finalB = 255.0f * pow(finalB / 255.0f, gammaTarget);

    float normR = finalR / 255.0f;
    float normG = finalG / 255.0f;
    float normB = finalB / 255.0f;

    normR = normR * normR * (3.0f - 2.0f * normR);
    normG = normG * normG * (3.0f - 2.0f * normG);
    normB = normB * normB * (3.0f - 2.0f * normB);

    finalR = normR * 255.0f;
    finalG = normG * 255.0f;
    finalB = normB * 255.0f;

    colourInfo[i].R = static_cast<uint8_t>(finalR + 0.5f);
    colourInfo[i].G = static_cast<uint8_t>(finalG + 0.5f);
    colourInfo[i].B = static_cast<uint8_t>(finalB + 0.5f);
}

    ofstream outImage ("..\\test\\pure_sensor1.ppm", ios::binary);   

    outImage<<"P6\n"<<width<<" "<<height<<"\n255\n";

    for(size_t i = 0; i<pixelData.size();i++)
    {
        outImage.put(colourInfo[i].R);
        outImage.put(colourInfo[i].G);
        outImage.put(colourInfo[i].B);
    }

    outImage.close();
    return 0;
}