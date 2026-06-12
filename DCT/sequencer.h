#include"..\\YCbCr-downsampler\\CbCr_downsampler.h"
#include <array>

using namespace std;

int8_t* gridTransform(vector<uint8_t>&a, int i)
{
    
}

vector<array<int8_t,64>> DCTmethod(vector<uint8_t> &x, int w, int h)
{
    vector<array<int8_t,64>> y(w*h/64);

    for(int row = 0; row<h ; row = row +8)
    {
        for(int col = 0; col<w; col+8)
        {

        }
    }
}
