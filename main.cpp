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
    string dummyFilePath = ".\\dummy_file\\dummy_file.jpg";
    string filepath;

    cout<<"Enter the file path: ";
    cin>>filepath;

    ifstream dummy (dummyFilePath, ios::binary);
    if(!dummy.is_open())
    {
        cout<<"failed here";
        return 0;
    }

    char headers [(38*16)+15];

    dummy.read(headers, (38*16)+15);

    dummy.close();

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

    Cb = compressedCbchannel(filepath, &width, &height);
    cout<<Cb.size()<<endl;

    Y = compressedYchannel(filepath, &width, &height);
    cout<<Y.size()<<endl;

    Cr = compressedCrchannel(filepath, &width, &height);
    cout<<Cr.size()<<endl;

    height = height/8;
    width = width/8;


    ofstream myJpg ("compressed.jpg", ios::binary);
    myJpg.write(headers, (38*16)+15);

    uint32_t buffer = 1;

    int16_t prev_Y_DC = 0;
    int16_t prev_Cb_DC = 0;
    int16_t prev_Cr_DC = 0;

    for(int row = 0; row<height; row = row +2)
    {   
        int nextRow = min(row + 1, height - 1);
        cout << "Processing Row: " << row <<endl;
        for(int col = 0; col<width; col = col +2)
        {   
            int nextCol = min(col + 1, width - 1);
            //DCpush
            int16_t current_Y1 = Y[(row*width)+col][0].value;
            int16_t delta_Y1 = current_Y1 - prev_Y_DC;
            prev_Y_DC = current_Y1;
            uint8_t DCvalueSize = calculateSize(delta_Y1);
            LeftAlignedPrefix DCprefix = getLumaDCPrefix(DCvalueSize);
            DCprefix.bits= DCprefix.bits<<1;
            for(int i = 0; i<(32-DCprefix.trailingZeroes-1); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (DCprefix.bits&0x80000000)>>31;
                while(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    myJpg.put(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        myJpg.put(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCprefix.bits = DCprefix.bits<<1;
            }
            uint16_t DCvalue;
            if(delta_Y1 >= 0) {
                DCvalue = delta_Y1;
            } else {
                DCvalue = ~(-delta_Y1);
            }
            DCvalue = DCvalue<<(16-DCvalueSize);
            for(int i = 0; i<(DCvalueSize); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (((uint32_t)DCvalue<<16)&0x80000000)>>31;
                while(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    myJpg.put(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        myJpg.put(0x00); 
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
                    while(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        myJpg.put(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            myJpg.put(0x00); 
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
                    while(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        myJpg.put(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            myJpg.put(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACvalue = ACvalue<<1;
                }
            }







            //DCpush
            int16_t current_Y2 = Y[(row*width)+nextCol][0].value;
            int16_t delta_Y2 = current_Y2 - prev_Y_DC;
            prev_Y_DC = current_Y2;
            DCvalueSize = calculateSize(delta_Y2);
            DCprefix = getLumaDCPrefix(DCvalueSize);
            DCprefix.bits= DCprefix.bits<<1;
            for(int i = 0; i<(32-DCprefix.trailingZeroes-1); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (DCprefix.bits&0x80000000)>>31;
                while(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    myJpg.put(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        myJpg.put(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCprefix.bits = DCprefix.bits<<1;
            }
            if(delta_Y2 >= 0) {
                DCvalue = delta_Y2;
            } else {
                DCvalue = ~(-delta_Y2);
            }
            DCvalue = DCvalue<<(16-DCvalueSize);
            for(int i = 0; i<(DCvalueSize); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (((uint32_t)DCvalue<<16)&0x80000000)>>31;
                while(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    myJpg.put(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        myJpg.put(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCvalue = DCvalue<<1;
            }

            //AC push

            for(int i = 1; i<Y[(row*width)+nextCol].size(); i++)
            {
                uint8_t ACvalueSize = calculateSize(Y[(row*width)+nextCol][i].value);
                LeftAlignedPrefix ACprefix = getLumaACPrefix(Y[(row*width)+nextCol][i].runLength,ACvalueSize);
                ACprefix.bits= ACprefix.bits<<1;
                for(int j = 0; j<(32-ACprefix.trailingZeroes-1); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (ACprefix.bits&0x80000000)>>31;
                    while(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        myJpg.put(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            myJpg.put(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACprefix.bits = ACprefix.bits<<1;
                }
                if(Y[(row*width)+nextCol][i].value == 0 && Y[(row*width)+nextCol][i].runLength == 0)
                {
                    continue;
                }
                uint16_t ACvalue;
                if(Y[(row*width)+nextCol][i].value>=0)
                {
                    ACvalue = Y[(row*width)+nextCol][i].value;
                } else if(Y[(row*width)+nextCol][i].value<0)
                {
                    Y[(row*width)+nextCol][i].value = -Y[(row*width)+nextCol][i].value;
                    ACvalue = Y[(row*width)+nextCol][i].value;
                    ACvalue = ~ACvalue;
                }
                ACvalue = ACvalue<<(16-ACvalueSize);
                for(int j = 0; j<(ACvalueSize); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (((uint32_t)ACvalue<<16)&0x80000000)>>31;
                    while(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        myJpg.put(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            myJpg.put(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACvalue = ACvalue<<1;
                }
            }
            
            




            //DCpush
            int16_t current_Y3 = Y[((nextRow)*width)+col][0].value;
            int16_t delta_Y3 = current_Y3 - prev_Y_DC;
            prev_Y_DC = current_Y3;
            DCvalueSize = calculateSize(delta_Y3);
            DCprefix = getLumaDCPrefix(DCvalueSize);
            DCprefix.bits= DCprefix.bits<<1;
            for(int i = 0; i<(32-DCprefix.trailingZeroes-1); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (DCprefix.bits&0x80000000)>>31;
                while(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    myJpg.put(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        myJpg.put(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCprefix.bits = DCprefix.bits<<1;
            }
            if(delta_Y3 >= 0) {
                DCvalue = delta_Y3;
            } else {
                DCvalue = ~(-delta_Y3);
            }
            DCvalue = DCvalue<<(16-DCvalueSize);
            for(int i = 0; i<(DCvalueSize); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (((uint32_t)DCvalue<<16)&0x80000000)>>31;
                while(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    myJpg.put(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        myJpg.put(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCvalue = DCvalue<<1;
            }

            //AC push

            for(int i = 1; i<Y[((nextRow)*width)+col].size(); i++)
            {
                uint8_t ACvalueSize = calculateSize(Y[((nextRow)*width)+col][i].value);
                LeftAlignedPrefix ACprefix = getLumaACPrefix(Y[((nextRow)*width)+col][i].runLength,ACvalueSize);
                ACprefix.bits= ACprefix.bits<<1;
                for(int j = 0; j<(32-ACprefix.trailingZeroes-1); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (ACprefix.bits&0x80000000)>>31;
                    while(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        myJpg.put(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            myJpg.put(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACprefix.bits = ACprefix.bits<<1;
                }
                if(Y[((nextRow)*width)+col][i].value == 0 && Y[((nextRow)*width)+col][i].runLength == 0)
                {
                    continue;
                }
                uint16_t ACvalue;
                if(Y[((nextRow)*width)+col][i].value>=0)
                {
                    ACvalue = Y[((nextRow)*width)+col][i].value;
                } else if(Y[((nextRow)*width)+col][i].value<0)
                {
                    Y[((nextRow)*width)+col][i].value = -Y[((nextRow)*width)+col][i].value;
                    ACvalue = Y[((nextRow)*width)+col][i].value;
                    ACvalue = ~ACvalue;
                }
                ACvalue = ACvalue<<(16-ACvalueSize);
                for(int j = 0; j<(ACvalueSize); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (((uint32_t)ACvalue<<16)&0x80000000)>>31;
                    while(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        myJpg.put(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            myJpg.put(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACvalue = ACvalue<<1;
                }
            }






            //DCpush
            int16_t current_Y4 = Y[((nextRow)*width)+nextCol][0].value;
            int16_t delta_Y4 = current_Y4 - prev_Y_DC;
            prev_Y_DC = current_Y4;
            DCvalueSize = calculateSize(delta_Y4);
            DCprefix = getLumaDCPrefix(DCvalueSize);
            DCprefix.bits= DCprefix.bits<<1;
            for(int i = 0; i<(32-DCprefix.trailingZeroes-1); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (DCprefix.bits&0x80000000)>>31;
                while(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    myJpg.put(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        myJpg.put(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCprefix.bits = DCprefix.bits<<1;
            }
            if(delta_Y4 >= 0) {
                DCvalue = delta_Y4;
            } else {
                DCvalue = ~(-delta_Y4);
            }
            DCvalue = DCvalue<<(16-DCvalueSize);
            for(int i = 0; i<(DCvalueSize); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (((uint32_t)DCvalue<<16)&0x80000000)>>31;
                while(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    myJpg.put(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        myJpg.put(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCvalue = DCvalue<<1;
            }

            //AC push

            for(int i = 1; i<Y[((nextRow)*width)+nextCol].size(); i++)
            {
                uint8_t ACvalueSize = calculateSize(Y[((nextRow)*width)+nextCol][i].value);
                LeftAlignedPrefix ACprefix = getLumaACPrefix(Y[((nextRow)*width)+nextCol][i].runLength,ACvalueSize);
                ACprefix.bits= ACprefix.bits<<1;
                for(int j = 0; j<(32-ACprefix.trailingZeroes-1); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (ACprefix.bits&0x80000000)>>31;
                    while(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        myJpg.put(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            myJpg.put(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACprefix.bits = ACprefix.bits<<1;
                }
                if(Y[((nextRow)*width)+nextCol][i].value == 0 && Y[((nextRow)*width)+nextCol][i].runLength == 0)
                {
                    continue;
                }
                uint16_t ACvalue;
                if(Y[((nextRow)*width)+nextCol][i].value>=0)
                {
                    ACvalue = Y[((nextRow)*width)+nextCol][i].value;
                } else if(Y[((nextRow)*width)+nextCol][i].value<0)
                {
                    Y[((nextRow)*width)+nextCol][i].value = -Y[((nextRow)*width)+nextCol][i].value;
                    ACvalue = Y[((nextRow)*width)+nextCol][i].value;
                    ACvalue = ~ACvalue;
                }
                ACvalue = ACvalue<<(16-ACvalueSize);
                for(int j = 0; j<(ACvalueSize); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (((uint32_t)ACvalue<<16)&0x80000000)>>31;
                    while(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        myJpg.put(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            myJpg.put(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACvalue = ACvalue<<1;
                }
            }





            //DCpush
            int16_t current_Cb = Cb[((row/2)*(width/2))+(col/2)][0].value;
            int16_t delta_Cb = current_Cb - prev_Cb_DC;
            prev_Cb_DC = current_Cb;
            DCvalueSize = calculateSize(delta_Cb);
            DCprefix = getChromaDCPrefix(DCvalueSize);
            DCprefix.bits= DCprefix.bits<<1;
            for(int i = 0; i<(32-DCprefix.trailingZeroes-1); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (DCprefix.bits&0x80000000)>>31;
                while(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    myJpg.put(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        myJpg.put(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCprefix.bits = DCprefix.bits<<1;
            }
            if(delta_Cb >= 0) {
                DCvalue = delta_Cb;
            } else {
                DCvalue = ~(-delta_Cb);
            }
            DCvalue = DCvalue<<(16-DCvalueSize);
            for(int i = 0; i<(DCvalueSize); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (((uint32_t)DCvalue<<16)&0x80000000)>>31;
                while(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    myJpg.put(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        myJpg.put(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCvalue = DCvalue<<1;
            }

            //AC push

            for(int i = 1; i<Cb[((row/2)*(width/2))+(col/2)].size(); i++)
            {
                uint8_t ACvalueSize = calculateSize(Cb[((row/2)*(width/2))+(col/2)][i].value);
                LeftAlignedPrefix ACprefix = getChromaACPrefix(Cb[((row/2)*(width/2))+(col/2)][i].runLength,ACvalueSize);
                ACprefix.bits= ACprefix.bits<<1;
                for(int j = 0; j<(32-ACprefix.trailingZeroes-1); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (ACprefix.bits&0x80000000)>>31;
                    while(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        myJpg.put(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            myJpg.put(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACprefix.bits = ACprefix.bits<<1;
                }
                if(Cb[((row/2)*(width/2))+(col/2)][i].value == 0 && Cb[((row/2)*(width/2))+(col/2)][i].runLength == 0)
                {
                    continue;
                }
                uint16_t ACvalue;
                if(Cb[((row/2)*(width/2))+(col/2)][i].value>=0)
                {
                    ACvalue = Cb[((row/2)*(width/2))+(col/2)][i].value;
                } else if(Cb[((row/2)*(width/2))+(col/2)][i].value<0)
                {
                    Cb[((row/2)*(width/2))+(col/2)][i].value = -Cb[((row/2)*(width/2))+(col/2)][i].value;
                    ACvalue = Cb[((row/2)*(width/2))+(col/2)][i].value;
                    ACvalue = ~ACvalue;
                }
                ACvalue = ACvalue<<(16-ACvalueSize);
                for(int j = 0; j<(ACvalueSize); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (((uint32_t)ACvalue<<16)&0x80000000)>>31;
                    while(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        myJpg.put(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            myJpg.put(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACvalue = ACvalue<<1;
                }
            }






            //DCpush
            int16_t current_Cr = Cr[((row/2)*(width/2))+(col/2)][0].value;
            int16_t delta_Cr = current_Cr - prev_Cr_DC;
            prev_Cr_DC = current_Cr;
            DCvalueSize = calculateSize(delta_Cr);
            DCprefix = getChromaDCPrefix(DCvalueSize);
            DCprefix.bits= DCprefix.bits<<1;
            for(int i = 0; i<(32-DCprefix.trailingZeroes-1); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (DCprefix.bits&0x80000000)>>31;
                while(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    myJpg.put(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        myJpg.put(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCprefix.bits = DCprefix.bits<<1;
            }
            if(delta_Cr >= 0) {
                DCvalue = delta_Cr;
            } else {
                DCvalue = ~(-delta_Cr);
            }
            DCvalue = DCvalue<<(16-DCvalueSize);
            for(int i = 0; i<(DCvalueSize); i++)
            {
                buffer = buffer<<1;
                buffer = buffer | (((uint32_t)DCvalue<<16)&0x80000000)>>31;
                while(calculateSize(buffer)>8)
                {   
                    uint8_t brightCheck = 0xFF;
                    myJpg.put(buffer);
                    if((uint8_t)buffer == brightCheck)
                    {
                        myJpg.put(0x00); 
                    }
                    buffer = buffer>>8;
                }
                DCvalue = DCvalue<<1;
            }

            //AC push

            for(int i = 1; i<Cr[((row/2)*(width/2))+(col/2)].size(); i++)
            {
                uint8_t ACvalueSize = calculateSize(Cr[((row/2)*(width/2))+(col/2)][i].value);
                LeftAlignedPrefix ACprefix = getChromaACPrefix(Cr[((row/2)*(width/2))+(col/2)][i].runLength,ACvalueSize);
                ACprefix.bits= ACprefix.bits<<1;
                for(int j = 0; j<(32-ACprefix.trailingZeroes-1); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (ACprefix.bits&0x80000000)>>31;
                    while(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        myJpg.put(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            myJpg.put(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACprefix.bits = ACprefix.bits<<1;
                }
                if(Cr[((row/2)*(width/2))+(col/2)][i].value == 0 && Cr[((row/2)*(width/2))+(col/2)][i].runLength == 0)
                {
                    continue;
                }
                uint16_t ACvalue;
                if(Cr[((row/2)*(width/2))+(col/2)][i].value>=0)
                {
                    ACvalue = Cr[((row/2)*(width/2))+(col/2)][i].value;
                } else if(Cr[((row/2)*(width/2))+(col/2)][i].value<0)
                {
                    Cr[((row/2)*(width/2))+(col/2)][i].value = -Cr[((row/2)*(width/2))+(col/2)][i].value;
                    ACvalue = Cr[((row/2)*(width/2))+(col/2)][i].value;
                    ACvalue = ~ACvalue;
                }
                ACvalue = ACvalue<<(16-ACvalueSize);
                for(int j = 0; j<(ACvalueSize); j++)
                {
                    buffer = buffer<<1;
                    buffer = buffer | (((uint32_t)ACvalue<<16)&0x80000000)>>31;
                    while(calculateSize(buffer)>8)
                    {   
                        uint8_t brightCheck = 0xFF;
                        myJpg.put(buffer);
                        if((uint8_t)buffer == brightCheck)
                        {
                            myJpg.put(0x00); 
                        }
                        buffer = buffer>>8;
                    }
                    ACvalue = ACvalue<<1;
                }
            }

        }
    }

    while(calculateSize(buffer)<=8)
    {
        buffer=buffer<<1;
        buffer = buffer | 1;
    }
    myJpg.put(buffer);
    if((uint8_t)buffer == 0xFF)
    {
        myJpg.put(0x00);
    }

    myJpg.put(0xFF);
    myJpg.put(0xD9);
    myJpg.close();
    return 0;
}