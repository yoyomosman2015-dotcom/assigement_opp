#include "image_class.h"
#include <string>
using namespace std;
void filternum10(const string& image_path, Image& image)
{
    for (int x = 0 ; x < image.width ; x++)
        for (int y = 0 ; y < image.height ; y++)
        {
            int result = 0;
            for (int z = 0 ; z < image.channels ; z++)
            {
                result += image.getPixel(x, y, z);
            }
                result = result/3;
                image.setPixel(x,y,0,result);
                image.setPixel(x,y,1,result);
                image.setPixel(x,y,2,result);
        }
            Image newOne(image.width, image.height);
        //  image.saveImage("BlackandWhite.png");
        for (int i = 0 ; i < image.width-1 ; i++)
            for (int j = 0 ; j < image.height-1 ; j++)
            {
            int diff1 = abs(image(i,j,0)-image(i+1,j+1,0));
            // int diff2 = abs(image(i,j,0)-image(i,j+1,0));

                if (diff1>40)
                {
                    newOne.setPixel(i,j,0,0);
                    newOne.setPixel(i,j,1,0);
                    newOne.setPixel(i,j,2,0);
                }
                else {
                    newOne.setPixel(i,j,0,255);
                    newOne.setPixel(i,j,1,255);
                    newOne.setPixel(i,j,2,255);
                }
            }
               image = newOne;
               image.saveImage("marked.png");
}


int main () {
    cout << "plz inter your image" << "\n";
    string s;
    cin >> s;
    Image image(s);
    filternum10(s,image);
      
}