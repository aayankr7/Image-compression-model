#include <iostream>
#include<fstream>
#include<iomanip>
using namespace std;

int main()
{
    ifstream image ("..\\test\\testImage.ARW", ios::binary);
    if(!image.is_open())
    {
        cout<<"image not opened"<<endl;
        return 1;
    }

    char offsetBytes[4];
    char directoryHeader[2];
    char test[4];
    int j = 0;
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
    
    while(offsetDirectoryHeader!=0x0111)
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
    uint32_t stripOffsets = offset;

    image.read(directoryHeader, 2);
    offsetDirectoryHeader = ((unsigned char)directoryHeader[1]<<8)|
                            ((unsigned char)directoryHeader[0]);
    image.seekg(SubIFD + 2, ios::beg);

    image.read(directoryHeader, 2);
    offsetDirectoryHeader = ((unsigned char)directoryHeader[1]<<8)|
                            ((unsigned char)directoryHeader[0]);

    while(offsetDirectoryHeader!=0x0117)
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
    uint32_t stripByteCounts = offset;

    cout<<stripByteCounts;
}