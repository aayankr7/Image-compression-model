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

    image.seekg(offset, ios::beg);

    uint16_t offsetDirectoryHeader;
    image.read(directoryHeader, 2);
    offsetDirectoryHeader = ((unsigned char)directoryHeader[1]<<8)|
                            ((unsigned char)directoryHeader[0]);
    
    uint16_t temp = offsetDirectoryHeader;                      
    for(uint16_t i = 0x0000; i<temp; i++)
    {
        image.read(directoryHeader, 2);
        offsetDirectoryHeader = ((unsigned char)directoryHeader[1]<<8)|
                                ((unsigned char)directoryHeader[0]);
        cout<<hex
            <<setw(4)
            <<setfill('0')
            <<offsetDirectoryHeader<<endl;
        image.seekg(10, ios::cur);
    }
}