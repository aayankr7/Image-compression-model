#pragma once
#include "..\\YCbCr-downsampler\\folder'sOutput.h"
#include "zigzag.h"
using namespace std;

vector<array<int16_t,64>> finalYchannel (string filepath, int*w, int*h)
{
    int width, height;
    uint32_t stripBytesCount;
    if(check(filepath, &stripBytesCount))
    {
        return vector<array<int16_t,64>>();
    }
    vector<uint8_t> LumaData(stripBytesCount);
    LumaData = Ychannel(filepath, &width, &height);

    cout<<"Sequencing Y data"<<endl;

    vector<array<int8_t,64>> sequencedLumaData((width*height)/64);
    sequencedLumaData = sequence(LumaData, width, height);
    LumaData.erase(LumaData.begin(), LumaData.end());

    cout<<"DCT Y data(this can take 3 minutes)"<<endl;

    vector<array<int16_t,64>> coefficientsOfLuma(sequencedLumaData.size());
    coefficientsOfLuma = coefficients(sequencedLumaData);
    uint32_t size = sequencedLumaData.size();
    sequencedLumaData.erase(sequencedLumaData.begin(),sequencedLumaData.end());

    cout<<"Quantizing Y data"<<endl;

    vector<array<int16_t,64>> quantizedCoefficientsOfLuma(size);
    quantizedCoefficientsOfLuma = quantized(coefficientsOfLuma, 1);
    coefficientsOfLuma.erase(coefficientsOfLuma.begin(),coefficientsOfLuma.end());

    cout<<"Reading Y data in ZigZag"<<endl;

    vector<array<int16_t,64>> ZigzagQuantizedCoefficientsOfLuma(size);
    ZigzagQuantizedCoefficientsOfLuma = zigZagSequence(quantizedCoefficientsOfLuma);
    quantizedCoefficientsOfLuma.erase(quantizedCoefficientsOfLuma.begin(),quantizedCoefficientsOfLuma.end());

    *w = width;
    *h = height;
    return ZigzagQuantizedCoefficientsOfLuma;

}

vector<array<int16_t,64>> finalCrchannel (string filepath, int*w, int*h)
{
    int width, height;
    uint32_t stripBytesCount;
    if(check(filepath, &stripBytesCount))
    {
        return vector<array<int16_t,64>>();
    }
    vector<uint8_t> CrData(stripBytesCount/4);
    CrData = Crchannel(filepath, &width, &height);

    cout<<"Sequencing Cr data"<<endl;

    vector<array<int8_t,64>> sequencedCrData(stripBytesCount/(64*4));
    sequencedCrData = sequence(CrData, width/2, height/2);
    CrData.erase(CrData.begin(), CrData.end());

    cout<<"DCT Cr data(this can take a minute)"<<endl;

    vector<array<int16_t,64>> coefficientsOfCr(sequencedCrData.size());
    coefficientsOfCr = coefficients(sequencedCrData);
    uint32_t size = sequencedCrData.size();
    sequencedCrData.erase(sequencedCrData.begin(),sequencedCrData.end());

    cout<<"Quantizing Cr data"<<endl;

    vector<array<int16_t,64>> quantizedCoefficientsOfCr(size);
    quantizedCoefficientsOfCr = quantized(coefficientsOfCr, 0);
    coefficientsOfCr.erase(coefficientsOfCr.begin(),coefficientsOfCr.end());

    cout<<"Reading Cr data in Zigzag"<<endl;

    vector<array<int16_t,64>> ZigzagQuantizedCoefficientsOfCr(size);
    ZigzagQuantizedCoefficientsOfCr = zigZagSequence(quantizedCoefficientsOfCr);
    quantizedCoefficientsOfCr.erase(quantizedCoefficientsOfCr.begin(),quantizedCoefficientsOfCr.end());

    *w = width;
    *h = height;
    return ZigzagQuantizedCoefficientsOfCr;

}

vector<array<int16_t,64>> finalCbchannel (string filepath, int*w, int*h)
{
    int width, height;
    uint32_t stripBytesCount;
    if(check(filepath, &stripBytesCount))
    {
        return vector<array<int16_t,64>>();
    }
    vector<uint8_t> CbData(stripBytesCount/4);
    CbData = Cbchannel(filepath, &width, &height);

    cout<<"Sequencing Cb data"<<endl;

    vector<array<int8_t,64>> sequencedCbData(stripBytesCount/(64*4));
    sequencedCbData = sequence(CbData, width/2, height/2);
    CbData.erase(CbData.begin(), CbData.end());

    cout<<"DCT Cb data(this can take a minute)"<<endl;

    vector<array<int16_t,64>> coefficientsOfCb(sequencedCbData.size());
    coefficientsOfCb = coefficients(sequencedCbData);
    uint32_t size = sequencedCbData.size();
    sequencedCbData.erase(sequencedCbData.begin(),sequencedCbData.end());

    cout<<"Quantizing Cb data"<<endl;

    vector<array<int16_t,64>> quantizedCoefficientsOfCb(size);
    quantizedCoefficientsOfCb = quantized(coefficientsOfCb, 0);
    coefficientsOfCb.erase(coefficientsOfCb.begin(),coefficientsOfCb.end());

    cout<<"Reading Cb data in Zigzag"<<endl;

    vector<array<int16_t,64>> ZigzagQuantizedCoefficientsOfCb(size);
    ZigzagQuantizedCoefficientsOfCb = zigZagSequence(quantizedCoefficientsOfCb);
    quantizedCoefficientsOfCb.erase(quantizedCoefficientsOfCb.begin(),quantizedCoefficientsOfCb.end());

    *w = width;
    *h = height;
    return ZigzagQuantizedCoefficientsOfCb;

}

