#pragma once
#include"CbCr_downsampler.h"
#include"..\\Parser+RGBconverter\\folder'sOutput.h"

vector<uint8_t> Ychannel(string filepath, int*w, int*h)
{   
    cout<<"converting RGB to Y"<<endl;
    uint32_t stripBytesCount;
    if(check(filepath, &stripBytesCount))
    {
        return vector<uint8_t>();
    }
    vector<struct colour> RGBdata(stripBytesCount);
    int width, height;
    RGBdata = RGBpixels(filepath, &width, &height);

    vector<struct colour2> YCbCr(stripBytesCount);
    YCbCr = YCbCrData(RGBdata);
    RGBdata.erase(RGBdata.begin(), RGBdata.end());

    vector<uint8_t> LumaData(stripBytesCount);
    LumaData = Luma(YCbCr, width, height);
    YCbCr.erase(YCbCr.begin(),YCbCr.end());

    *w = width;
    *h = height;
    return LumaData;
}

vector<uint8_t> Cbchannel(string filepath, int*w, int*h)
{   
    cout<<"converting RGB to Cb"<<endl;
    uint32_t stripBytesCount;
    if(check(filepath, &stripBytesCount))
    {
        return vector<uint8_t>();
    }
    vector<struct colour> RGBdata(stripBytesCount);
    int width, height;
    RGBdata = RGBpixels(filepath, &width, &height);

    vector<struct colour2> YCbCr(stripBytesCount);
    YCbCr = YCbCrData(RGBdata);
    RGBdata.erase(RGBdata.begin(), RGBdata.end());

    vector<uint8_t> CbData(stripBytesCount/4);
    CbData = ChromaBlue(YCbCr, width, height);
    YCbCr.erase(YCbCr.begin(),YCbCr.end());

    *w = width;
    *h = height;
    return CbData;
}

vector<uint8_t> Crchannel(string filepath, int*w, int*h)
{   
    cout<<"converting RGB to Cr"<<endl;
    uint32_t stripBytesCount;
    if(check(filepath, &stripBytesCount))
    {
        return vector<uint8_t>();
    }
    vector<struct colour> RGBdata(stripBytesCount);
    int width, height;
    RGBdata = RGBpixels(filepath, &width, &height);

    vector<struct colour2> YCbCr(stripBytesCount);
    YCbCr = YCbCrData(RGBdata);
    RGBdata.erase(RGBdata.begin(), RGBdata.end());

    vector<uint8_t> CrData(stripBytesCount/4);
    CrData = ChromaRed(YCbCr, width, height);
    YCbCr.erase(YCbCr.begin(),YCbCr.end());

    *w = width;
    *h = height;
    return CrData;
}