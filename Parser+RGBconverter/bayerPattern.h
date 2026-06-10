#pragma once
#include "grayscaleQuantization.h"
using namespace std;

vector<int> bayerPattern(string filepath)
{
    vector<int> y(4);
    ifstream image (filepath, ios::binary);
    if(!image.is_open())
    {
        cout<<"image not opened"<<endl;
        return vector<int>();
    }

    char offsetBytes[4];
    char directoryHeader[2];
    uint32_t offset;

    image.seekg(4, ios::beg);
    image.read(offsetBytes,4);

    offset = ((unsigned char)offsetBytes[3]<<24)|
             ((unsigned char)offsetBytes[2]<<16)|
             ((unsigned char)offsetBytes[1]<<8)|
             ((unsigned char)offsetBytes[0]);

    image.seekg(offset+2, ios::beg);

    uint16_t offsetDirectoryHeader;
    image.read(directoryHeader, 2);
    offsetDirectoryHeader = ((unsigned char)directoryHeader[1]<<8)|
                            ((unsigned char)directoryHeader[0]);

    while(offsetDirectoryHeader!=0x014A)
    {
        image.seekg(10, ios::cur);
        image.read(directoryHeader, 2);
        offsetDirectoryHeader = ((unsigned char)directoryHeader[1]<<8)|
                                ((unsigned char)directoryHeader[0]);
    }

    image.seekg(6, ios::cur);
    image.read(offsetBytes, 4);
    offset = ((unsigned char)offsetBytes[3]<<24)|
             ((unsigned char)offsetBytes[2]<<16)|
             ((unsigned char)offsetBytes[1]<<8)|
             ((unsigned char)offsetBytes[0]);
    uint32_t SubIFD = offset;
    image.seekg(offset+2, ios::beg);

    image.read(directoryHeader, 2);
    offsetDirectoryHeader = ((unsigned char)directoryHeader[1]<<8)|
                            ((unsigned char)directoryHeader[0]);
    int j = 0;

    while(offsetDirectoryHeader!=0x828E)
    {
        image.seekg(10, ios::cur);
        image.read(directoryHeader, 2);
        offsetDirectoryHeader = ((unsigned char)directoryHeader[1]<<8)|
                                ((unsigned char)directoryHeader[0]);
        j++;
        if(j>10000)
        {   
            cout<<"didnt find it";
            break;
            return vector<int>();
        }
    }

    image.seekg(6, ios::cur);
    image.read(offsetBytes, 4);

    for(int i =0; i<4; i++)
    {
        y[i] = offsetBytes[i];
    }

    return y;
}