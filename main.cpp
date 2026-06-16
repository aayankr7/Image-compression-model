#include ".\\FinalCompression\\folder'sOutput.h"
#include "huffmanPrefixes.h"

using namespace std;

uint8_t calculateSize(int16_t value)
{
    if (value == 0)
    {
        return 0;
    }else if (value < 0)
    {
        value = -value;
    }
    uint8_t size = 0;
    while (value > 0) {
        size++;
        value >>= 1;
    }   
    return size;
}

int main()
{
    string dummyFilePath = "..\\dummy_file\\dummy.jpg";
    string filepath;

    cout<<"Enter the file path: ";
    cin>>filepath;

    ifstream dummy (dummyFilePath, ios::binary);
    if(!dummy.is_open())
    {
        cout<<"failed here";
    }

    char headers [(38*16)+15];

    dummy.read(headers, (38*16)+15);

    initHuffmanTables();

    uint32_t stripByteCount;
    if(check(filepath,&stripByteCount))
    {
        return 0;
    }

    int width, height;

    cout<<"Now making the vector"<<endl;

    vector<vector<RLEpair>> Y (stripByteCount/64);
    vector<vector<RLEpair>> Cb (stripByteCount/(64*4));
    vector<vector<RLEpair>> Cr (stripByteCount/(64*4));

    Y = compressedYchannel(filepath, &width, &height);
    Cb = compressedCbchannel(filepath, &width, &height);
    Cr = compressedCrchannel(filepath, &width, &height);

    width = width/8;
    height = height/8;

    vector<uint8_t> imageData;
    uint32_t buffer = 1;

    for(int row = 0; row<(height); row = row +2)
    {
        cout << "Processing Row: " << row <<endl;
        for(int col = 0; col<(width); col = col +2)
        {   
            //DCpush
            uint8_t DCvalueSize = calculateSize(Y[(row*width)+col][0].value);
            LeftAlignedPrefix DCprefix = getLumaDCPrefix(DCvalueSize);
            DCprefix.bits= DCprefix.bits<<1;
            for(int i = 0; i<(32-DCprefix.trailingZeroes-1); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (DCprefix.bits&0x80000000)>>31;
                if(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    imageData.push_back(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        imageData.push_back(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCprefix.bits = DCprefix.bits<<1;
            }
            uint16_t DCvalue;
            if(Y[(row*width)+col][0].value>=0)
            {
                DCvalue = Y[(row*width)+col][0].value;
            } else if(Y[(row*width)+col][0].value<0)
            {
                Y[(row*width)+col][0].value = -Y[(row*width)+col][0].value;
                DCvalue = Y[(row*width)+col][0].value;
                DCvalue = ~DCvalue;
            }
            DCvalue = DCvalue<<(16-DCvalueSize);
            for(int i = 0; i<(DCvalueSize); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (((uint32_t)DCvalue<<16)&0x80000000)>>31;
                if(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    imageData.push_back(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        imageData.push_back(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCvalue = DCvalue<<1;
            }

            //AC push

            for(int i = 1; i<Y[(row*width)+col].size(); i++)
            {
                uint8_t ACvalueSize = calculateSize(Y[(row*width)+col][i].value);
                LeftAlignedPrefix ACprefix = getLumaACPrefix(Y[(row*width)+col][i].runLength,ACvalueSize);
                ACprefix.bits= ACprefix.bits<<1;
                for(int j = 0; j<(32-ACprefix.trailingZeroes-1); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (ACprefix.bits&0x80000000)>>31;
                    if(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        imageData.push_back(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            imageData.push_back(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACprefix.bits = ACprefix.bits<<1;
                }
                if(Y[(row*width)+col][i].value == 0 && Y[(row*width)+col][i].runLength == 0)
                {
                    continue;
                }
                uint16_t ACvalue;
                if(Y[(row*width)+col][i].value>=0)
                {
                    ACvalue = Y[(row*width)+col][i].value;
                } else if(Y[(row*width)+col][i].value<0)
                {
                    Y[(row*width)+col][i].value = -Y[(row*width)+col][i].value;
                    ACvalue = Y[(row*width)+col][i].value;
                    ACvalue = ~ACvalue;
                }
                ACvalue = ACvalue<<(16-ACvalueSize);
                for(int j = 0; j<(ACvalueSize); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (((uint32_t)ACvalue<<16)&0x80000000)>>31;
                    if(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        imageData.push_back(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            imageData.push_back(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACvalue = ACvalue<<1;
                }
            }







            //DCpush
            DCvalueSize = calculateSize(Y[(row*width)+col+1][0].value);
            DCprefix = getLumaDCPrefix(DCvalueSize);
            DCprefix.bits= DCprefix.bits<<1;
            for(int i = 0; i<(32-DCprefix.trailingZeroes-1); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (DCprefix.bits&0x80000000)>>31;
                if(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    imageData.push_back(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        imageData.push_back(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCprefix.bits = DCprefix.bits<<1;
            }
            if(Y[(row*width)+col+1][0].value>=0)
            {
                DCvalue = Y[(row*width)+col+1][0].value;
            } else if(Y[(row*width)+col+1][0].value<0)
            {
                Y[(row*width)+col+1][0].value = -Y[(row*width)+col+1][0].value;
                DCvalue = Y[(row*width)+col+1][0].value;
                DCvalue = ~DCvalue;
            }
            DCvalue = DCvalue<<(16-DCvalueSize);
            for(int i = 0; i<(DCvalueSize); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (((uint32_t)DCvalue<<16)&0x80000000)>>31;
                if(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    imageData.push_back(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        imageData.push_back(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCvalue = DCvalue<<1;
            }

            //AC push

            for(int i = 1; i<Y[(row*width)+col+1].size(); i++)
            {
                uint8_t ACvalueSize = calculateSize(Y[(row*width)+col+1][i].value);
                LeftAlignedPrefix ACprefix = getLumaACPrefix(Y[(row*width)+col+1][i].runLength,ACvalueSize);
                ACprefix.bits= ACprefix.bits<<1;
                for(int j = 0; j<(32-ACprefix.trailingZeroes-1); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (ACprefix.bits&0x80000000)>>31;
                    if(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        imageData.push_back(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            imageData.push_back(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACprefix.bits = ACprefix.bits<<1;
                }
                if(Y[(row*width)+col+1][i].value == 0 && Y[(row*width)+col+1][i].runLength == 0)
                {
                    continue;
                }
                uint16_t ACvalue;
                if(Y[(row*width)+col+1][i].value>=0)
                {
                    ACvalue = Y[(row*width)+col+1][i].value;
                } else if(Y[(row*width)+col+1][i].value<0)
                {
                    Y[(row*width)+col+1][i].value = -Y[(row*width)+col+1][i].value;
                    ACvalue = Y[(row*width)+col+1][i].value;
                    ACvalue = ~ACvalue;
                }
                ACvalue = ACvalue<<(16-ACvalueSize);
                for(int j = 0; j<(ACvalueSize); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (((uint32_t)ACvalue<<16)&0x80000000)>>31;
                    if(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        imageData.push_back(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            imageData.push_back(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACvalue = ACvalue<<1;
                }
            }
            
            




            //DCpush
            DCvalueSize = calculateSize(Y[((row+1)*width)+col][0].value);
            DCprefix = getLumaDCPrefix(DCvalueSize);
            DCprefix.bits= DCprefix.bits<<1;
            for(int i = 0; i<(32-DCprefix.trailingZeroes-1); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (DCprefix.bits&0x80000000)>>31;
                if(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    imageData.push_back(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        imageData.push_back(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCprefix.bits = DCprefix.bits<<1;
            }
            if(Y[((row+1)*width)+col][0].value>=0)
            {
                DCvalue = Y[((row+1)*width)+col][0].value;
            } else if(Y[((row+1)*width)+col][0].value<0)
            {
                Y[((row+1)*width)+col][0].value = -Y[((row+1)*width)+col][0].value;
                DCvalue = Y[((row+1)*width)+col][0].value;
                DCvalue = ~DCvalue;
            }
            DCvalue = DCvalue<<(16-DCvalueSize);
            for(int i = 0; i<(DCvalueSize); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (((uint32_t)DCvalue<<16)&0x80000000)>>31;
                if(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    imageData.push_back(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        imageData.push_back(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCvalue = DCvalue<<1;
            }

            //AC push

            for(int i = 1; i<Y[((row+1)*width)+col].size(); i++)
            {
                uint8_t ACvalueSize = calculateSize(Y[((row+1)*width)+col][i].value);
                LeftAlignedPrefix ACprefix = getLumaACPrefix(Y[((row+1)*width)+col][i].runLength,ACvalueSize);
                ACprefix.bits= ACprefix.bits<<1;
                for(int j = 0; j<(32-ACprefix.trailingZeroes-1); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (ACprefix.bits&0x80000000)>>31;
                    if(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        imageData.push_back(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            imageData.push_back(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACprefix.bits = ACprefix.bits<<1;
                }
                if(Y[((row+1)*width)+col][i].value == 0 && Y[((row+1)*width)+col][i].runLength == 0)
                {
                    continue;
                }
                uint16_t ACvalue;
                if(Y[((row+1)*width)+col][i].value>=0)
                {
                    ACvalue = Y[((row+1)*width)+col][i].value;
                } else if(Y[((row+1)*width)+col][i].value<0)
                {
                    Y[((row+1)*width)+col][i].value = -Y[((row+1)*width)+col][i].value;
                    ACvalue = Y[((row+1)*width)+col][i].value;
                    ACvalue = ~ACvalue;
                }
                ACvalue = ACvalue<<(16-ACvalueSize);
                for(int j = 0; j<(ACvalueSize); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (((uint32_t)ACvalue<<16)&0x80000000)>>31;
                    if(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        imageData.push_back(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            imageData.push_back(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACvalue = ACvalue<<1;
                }
            }






            //DCpush
            DCvalueSize = calculateSize(Y[((row+1)*width)+col+1][0].value);
            DCprefix = getLumaDCPrefix(DCvalueSize);
            DCprefix.bits= DCprefix.bits<<1;
            for(int i = 0; i<(32-DCprefix.trailingZeroes-1); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (DCprefix.bits&0x80000000)>>31;
                if(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    imageData.push_back(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        imageData.push_back(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCprefix.bits = DCprefix.bits<<1;
            }
            if(Y[((row+1)*width)+col+1][0].value>=0)
            {
                DCvalue = Y[((row+1)*width)+col+1][0].value;
            } else if(Y[((row+1)*width)+col+1][0].value<0)
            {
                Y[((row+1)*width)+col+1][0].value = -Y[((row+1)*width)+col+1][0].value;
                DCvalue = Y[((row+1)*width)+col+1][0].value;
                DCvalue = ~DCvalue;
            }
            DCvalue = DCvalue<<(16-DCvalueSize);
            for(int i = 0; i<(DCvalueSize); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (((uint32_t)DCvalue<<16)&0x80000000)>>31;
                if(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    imageData.push_back(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        imageData.push_back(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCvalue = DCvalue<<1;
            }

            //AC push

            for(int i = 1; i<Y[((row+1)*width)+col+1].size(); i++)
            {
                uint8_t ACvalueSize = calculateSize(Y[((row+1)*width)+col+1][i].value);
                LeftAlignedPrefix ACprefix = getLumaACPrefix(Y[((row+1)*width)+col+1][i].runLength,ACvalueSize);
                ACprefix.bits= ACprefix.bits<<1;
                for(int j = 0; j<(32-ACprefix.trailingZeroes-1); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (ACprefix.bits&0x80000000)>>31;
                    if(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        imageData.push_back(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            imageData.push_back(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACprefix.bits = ACprefix.bits<<1;
                }
                if(Y[((row+1)*width)+col+1][i].value == 0 && Y[((row+1)*width)+col+1][i].runLength == 0)
                {
                    continue;
                }
                uint16_t ACvalue;
                if(Y[((row+1)*width)+col+1][i].value>=0)
                {
                    ACvalue = Y[((row+1)*width)+col+1][i].value;
                } else if(Y[((row+1)*width)+col+1][i].value<0)
                {
                    Y[((row+1)*width)+col+1][i].value = -Y[((row+1)*width)+col+1][i].value;
                    ACvalue = Y[((row+1)*width)+col+1][i].value;
                    ACvalue = ~ACvalue;
                }
                ACvalue = ACvalue<<(16-ACvalueSize);
                for(int j = 0; j<(ACvalueSize); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (((uint32_t)ACvalue<<16)&0x80000000)>>31;
                    if(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        imageData.push_back(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            imageData.push_back(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACvalue = ACvalue<<1;
                }
            }





            //DCpush
            DCvalueSize = calculateSize(Cb[(row*(width/2))+(col/2)][0].value);
            DCprefix = getChromaDCPrefix(DCvalueSize);
            DCprefix.bits= DCprefix.bits<<1;
            for(int i = 0; i<(32-DCprefix.trailingZeroes-1); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (DCprefix.bits&0x80000000)>>31;
                if(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    imageData.push_back(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        imageData.push_back(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCprefix.bits = DCprefix.bits<<1;
            }
            if(Cb[(row*(width/2))+(col/2)][0].value>=0)
            {
                DCvalue = Cb[(row*(width/2))+(col/2)][0].value;
            } else if(Cb[(row*(width/2))+(col/2)][0].value<0)
            {
                Cb[(row*(width/2))+(col/2)][0].value = -Cb[(row*(width/2))+(col/2)][0].value;
                DCvalue = Cb[(row*(width/2))+(col/2)][0].value;
                DCvalue = ~DCvalue;
            }
            DCvalue = DCvalue<<(16-DCvalueSize);
            for(int i = 0; i<(DCvalueSize); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (((uint32_t)DCvalue<<16)&0x80000000)>>31;
                if(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    imageData.push_back(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        imageData.push_back(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCvalue = DCvalue<<1;
            }

            //AC push

            for(int i = 1; i<Cb[(row*(width/2))+(col/2)].size(); i++)
            {
                uint8_t ACvalueSize = calculateSize(Cb[(row*(width/2))+(col/2)][i].value);
                LeftAlignedPrefix ACprefix = getChromaACPrefix(Cb[(row*(width/2))+(col/2)][i].runLength,ACvalueSize);
                ACprefix.bits= ACprefix.bits<<1;
                for(int j = 0; j<(32-ACprefix.trailingZeroes-1); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (ACprefix.bits&0x80000000)>>31;
                    if(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        imageData.push_back(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            imageData.push_back(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACprefix.bits = ACprefix.bits<<1;
                }
                if(Cb[(row*(width/2))+(col/2)][i].value == 0 && Cb[(row*(width/2))+(col/2)][i].runLength == 0)
                {
                    continue;
                }
                uint16_t ACvalue;
                if(Cb[(row*(width/2))+(col/2)][i].value>=0)
                {
                    ACvalue = Cb[(row*(width/2))+(col/2)][i].value;
                } else if(Cb[(row*(width/2))+(col/2)][i].value<0)
                {
                    Cb[(row*(width/2))+(col/2)][i].value = -Cb[(row*(width/2))+(col/2)][i].value;
                    ACvalue = Cb[(row*(width/2))+(col/2)][i].value;
                    ACvalue = ~ACvalue;
                }
                ACvalue = ACvalue<<(16-ACvalueSize);
                for(int j = 0; j<(ACvalueSize); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (((uint32_t)ACvalue<<16)&0x80000000)>>31;
                    if(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        imageData.push_back(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            imageData.push_back(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACvalue = ACvalue<<1;
                }
            }






            //DCpush
            DCvalueSize = calculateSize(Cr[(row*(width/2))+(col/2)][0].value);
            DCprefix = getChromaDCPrefix(DCvalueSize);
            DCprefix.bits= DCprefix.bits<<1;
            for(int i = 0; i<(32-DCprefix.trailingZeroes-1); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (DCprefix.bits&0x80000000)>>31;
                if(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    imageData.push_back(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        imageData.push_back(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCprefix.bits = DCprefix.bits<<1;
            }
            if(Cr[(row*(width/2))+(col/2)][0].value>=0)
            {
                DCvalue = Cr[(row*(width/2))+(col/2)][0].value;
            } else if(Cr[(row*(width/2))+(col/2)][0].value<0)
            {
                Cr[(row*(width/2))+(col/2)][0].value = -Cr[(row*(width/2))+(col/2)][0].value;
                DCvalue = Cr[(row*(width/2))+(col/2)][0].value;
                DCvalue = ~DCvalue;
            }
            DCvalue = DCvalue<<(16-DCvalueSize);
            for(int i = 0; i<(DCvalueSize); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (((uint32_t)DCvalue<<16)&0x80000000)>>31;
                if(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    imageData.push_back(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        imageData.push_back(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCvalue = DCvalue<<1;
            }

            //AC push

            for(int i = 1; i<Cr[(row*(width/2))+(col/2)].size(); i++)
            {
                uint8_t ACvalueSize = calculateSize(Cr[(row*(width/2))+(col/2)][i].value);
                LeftAlignedPrefix ACprefix = getChromaACPrefix(Cr[(row*(width/2))+(col/2)][i].runLength,ACvalueSize);
                ACprefix.bits= ACprefix.bits<<1;
                for(int j = 0; j<(32-ACprefix.trailingZeroes-1); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (ACprefix.bits&0x80000000)>>31;
                    if(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        imageData.push_back(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            imageData.push_back(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACprefix.bits = ACprefix.bits<<1;
                }
                if(Cr[(row*(width/2))+(col/2)][i].value == 0 && Cr[(row*(width/2))+(col/2)][i].runLength == 0)
                {
                    continue;
                }
                uint16_t ACvalue;
                if(Cr[(row*(width/2))+(col/2)][i].value>=0)
                {
                    ACvalue = Cr[(row*(width/2))+(col/2)][i].value;
                } else if(Cr[(row*(width/2))+(col/2)][i].value<0)
                {
                    Cr[(row*(width/2))+(col/2)][i].value = -Cr[(row*(width/2))+(col/2)][i].value;
                    ACvalue = Cr[(row*(width/2))+(col/2)][i].value;
                    ACvalue = ~ACvalue;
                }
                ACvalue = ACvalue<<(16-ACvalueSize);
                for(int j = 0; j<(ACvalueSize); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (((uint32_t)ACvalue<<16)&0x80000000)>>31;
                    if(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        imageData.push_back(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            imageData.push_back(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACvalue = ACvalue<<1;
                }
            }

        }
    }

    while(calculateSize(buffer)>1)
    {
        buffer=buffer<<1;
        buffer = buffer | 1;
        if(calculateSize(buffer)>8)
        {   
            imageData.push_back(buffer);
            if((uint8_t)buffer == 0xFF)
            {
                imageData.push_back(0x00);
            }
            break;
        }
    }

    imageData.push_back(0xFF);
    imageData.push_back(0xD9);
    dummy.close();

    ofstream myJpg ("compressed.tfps", ios::binary);
    myJpg.write(headers, (38*16)+15);
    myJpg.write((char*)imageData.data(), imageData.size());
    myJpg.close();
    imageData.erase(imageData.begin(),imageData.end());

}