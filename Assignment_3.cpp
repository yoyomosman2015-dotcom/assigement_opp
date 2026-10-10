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
                  if (result>127) result = 255;
                  else result = 0;
                 image.setPixel(x,y,0,result);
                 image.setPixel(x,y,1,result);
                 image.setPixel(x,y,2,result);
        }
            Image newOne = image;
           for (int i = 1 ; i < newOne.width-1 ; i++)
                 for (int j = 1 ; j < newOne.height-1 ; j++){
                    if (image.getPixel(i,j-1,0)==0 && image.getPixel(i-1,j,0)==0 && image.getPixel(i+1,j,0)==0 && image.getPixel(i,j+1,0)==0){
                    newOne.setPixel(i,j,0,255);
                    newOne.setPixel(i,j,1,255);
                    newOne.setPixel(i,j,2,255);
                    }
                 }    
            newOne.saveImage("good.png");
}

void filternum11(Image& image,string s)
{
    cout << "plz inter the two dimensions (height, width) that you want to crop from\n";
    int x=0,y=0,W=0,H=0;
    bool check1 = (bool)(cin >> x >> y);
    while (!(check1)){
          cout << "invalid input plz inter an integer number\n";
          cin.clear();
          cin.ignore(1000,'\n');
          check1 = (bool)(cin >> x >> y);
    }
    while (x<0 || x>image.width || y<0 || y>image.height)
    {
        cout << "the dimensions are out of range\n";
        cout << "plz inter the correct dimensions which is greater than 0 and less than: " << image.width << " and " << image.height << "\n";
        // cin >> x >> y;
        bool check2 = (bool)(cin >> x >> y);
        while (!(check2)){
            cout << "invalid input plz inter an integer number\n";
            cin.clear();
            cin.ignore(1000,'\n');
            check2 = (bool)(cin >> x >> y);
        }
    }
    cout << "plz tell me the area that you want to cut\n";
    bool check3 = (bool)(cin >> W >> H);
    while (!(check3)){
          cout << "invalid input plz inter an integer number\n";
          cin.clear();
          cin.ignore(1000,'\n');
          check3 = (bool)(cin >> W >> H);
    }
    while (W<0 || H<0 || x+W>image.width || y+H>image.height)
    { 
        cout << "the area is out of range\n";
        cout << "plz inter the correct area which is greater than 0 and less than: " << image.width-x << " and " << image.height-y << "\n";
        // cin >> W >> H;
        bool check4 = (bool)(cin >> W >> H);
    while (!(check4)){
          cout << "invalid input plz inter an integer number\n";
          cin.clear();
          cin.ignore(1000,'\n');
          check4 = (bool)(cin >> W >> H);
    }
    }
    Image the_crop(W,H);
    for (int i=x;i<x+W;i++)
       for (int j=y;j<y+H;j++)
            for (int c=0;c<3;c++)
            {
                the_crop.setPixel(i-x,j-y,c,image.getPixel(i,j,c));
            }
            cout << "do you want to save the cropped image in a new image or in the same image , if new = y , not new = n?\n";
            string save;
            cin >> save;
            while (save!="y" && save!="n"){
                cout << "plz inter the correct input whith is y or n\n";
                cin >> save;
            }
            if (save=="y") the_crop.saveImage("good.png");
            else{
             image = the_crop;
             image.saveImage(s);
            }
}


void filternum14(Image& image){
    for (int i=0;i<image.width;i++)
       for (int j=0;j<image.height;j++){
       if (j%2!=0) continue;
       else for (int c=0;c<3;c++)
          {
              image.setPixel(i,j,c,(image.getPixel(i,j,c)*0.6));
          }
        }
          image.saveImage("good.png");
}





int main () {
    cout << "plz inter your image" << "\n";
    string s;
    cin >> s;
     Image image(s);
    cout << "inter fiter number (10,11,14)\n";
    int x;
    cin >> x;
    if (x==10) filternum10(s,image);
    else if (x==11) filternum11(image,s);
    else filternum14(image);

}