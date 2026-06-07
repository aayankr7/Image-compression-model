#include<fstream>
#include<iostream>
#include<iomanip>

using namespace std;

int main()
{
    ifstream image ("..\\testImage.ARW", ios::binary);
    if (!image.is_open()) {
        cerr << "Error: Could not open file! Check the file path." << endl;
        return 1;
    }
    char bytes[5];
    image.read(bytes, 5);

    for(int i = 0; i<5; i++)
    {
        cout<<hex
            <<setw(2)
            <<setfill('0')
            <<(unsigned int)(unsigned char)bytes[i]
            <<' ';
    }
}