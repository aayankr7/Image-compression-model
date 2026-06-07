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
        j++;
        if(j>1000)
        {
            cout<<"failed here";
            return 1;
        }
    }
    if(j<1000)
    {
        cout<<"found it";
    }

    /*image.seekg(6, ios::cur);
    image.read(offsetBytes,4);
    offset = ((unsigned char)offsetBytes[3]<<24)|
             ((unsigned char)offsetBytes[2]<<16)|
             ((unsigned char)offsetBytes[1]<<8)|
             ((unsigned char)offsetBytes[0]);
    image.seekg(offset, ios::beg);

    image.read(test, 4);
    for(int i = 0; i<4; i++)
    {
        cout<<hex
            <<setw(2)
            <<setfill('0')
            <<(unsigned int)(unsigned char)test[i]
            <<' ';
    }*/
}