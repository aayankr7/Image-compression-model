#pragma once
#include"headers.h"

vector<struct colour> RGBpixels(string filepath)
{
    uint32_t stripBytesCount;
    if(check(filepath, &stripBytesCount))
    {
        return vector<struct colour>();
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

    return colourInfo;
}