#pragma once
#include "ACshredding.h"
#include"..\\DCT\\folder'sOutput.h"

using namespace std;

vector<vector<RLEpair>> compressedYchannel(string filepath, int*w, int*h)
{
    int width, height;
    uint32_t stripBytesCount;

    cout<<"Compressing Y data"<<endl;
    if(check(filepath, &stripBytesCount))
    {
        return vector<vector<RLEpair>>();
    }
    vector<array<int16_t,64>> preDCTdata(stripBytesCount/64);
    preDCTdata = finalYchannel(filepath, &width, &height);

    vector<vector<RLEpair>> y(preDCTdata.size());
    y = ACshred(preDCTdata);
    preDCTdata.erase(preDCTdata.begin(), preDCTdata.end());
    *w = width;
    *h = height;
    return y;
}

vector<vector<RLEpair>> compressedCbchannel(string filepath, int*w, int*h)
{
    int width, height;
    uint32_t stripBytesCount;

    cout<<"Compressing Cb data"<<endl;
    if(check(filepath, &stripBytesCount))
    {
        return vector<vector<RLEpair>>();
    }
    vector<array<int16_t,64>> preDCTdata(stripBytesCount/(64*4));
    preDCTdata = finalCbchannel(filepath, &width, &height);

    vector<vector<RLEpair>> y(preDCTdata.size());
    y = ACshred(preDCTdata);
    preDCTdata.erase(preDCTdata.begin(), preDCTdata.end());
    *w = width;
    *h = height;
    return y;
}

vector<vector<RLEpair>> compressedCrchannel(string filepath, int*w, int*h)
{
    int width, height;
    uint32_t stripBytesCount;

    cout<<"Compressing Cr data"<<endl;
    if(check(filepath, &stripBytesCount))
    {
        return vector<vector<RLEpair>>();
    }
    vector<array<int16_t,64>> preDCTdata(stripBytesCount/(64*4));
    preDCTdata = finalCrchannel(filepath, &width, &height);

    vector<vector<RLEpair>> y(preDCTdata.size());
    y = ACshred(preDCTdata);
    preDCTdata.erase(preDCTdata.begin(), preDCTdata.end());
    *w = width;
    *h = height;
    return y;
}