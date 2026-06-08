#include<iostream>
#include"scavenger.h"
#include"formatChecker.h"
#include"bitConverter.h"
#include"grayscaleQuantization.h"
#include <vector>
#include <string>
#include <fstream>

int main()
{
    string filepath = "..\\test\\testImage1.ARW";
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

    ofstream outImage ("..\\test\\pure_sensor.pgm", ios::binary);   

    outImage<<"P5\n"<<width<<" "<<height<<"\n255\n";

    for(size_t i = 0; i<pixelData.size();i++)
    {
        
        char highByte = lossyPixels[i];
        outImage.put(highByte);
    }

    cout<<"hope it works"<<endl;
    outImage.close();
}