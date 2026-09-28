#include "image_class.h"
#include <string>
using namespace std;
void filternum6(const string& image_path, Image& image)
{
    Image newIma(image.height, image.width);
    
    for (int oldcol = 0 ; oldcol < image.width ; oldcol++)
        for (int oldrow = 0 ; oldrow < image.height ; oldrow++)
        {
            int newrow=0, newcol=0;
            newrow = oldcol;
            newcol = (image.height - 1) - oldrow;
            for (int c = 0 ; c < image.channels ; c++)
            {
                newIma.setPixel(newcol, newrow, c, image.getPixel(oldcol, oldrow, c));
            }
        }
        image = newIma;
        image.saveImage("good.png");
}


int main () {
    cout << "plz inter your image" << "\n";
    string s;
    cin >> s;
    Image image(s);
    cout << "what filter do you want to use? (1-8)" << "\n";
    int filter;
    cin >> filter;
    if (filter == 6)
    {
        cout << "you choose filter 6" << "\n";
        cout << "How many degrees do you want to rotate? (90, 180, 270)" << "\n";
        int degrees;
        cin >> degrees;
        if (degrees == 90)
            filternum6(s,image);
        else if (degrees == 180)
        {
            filternum6(s,image);
            filternum6(s,image);
        }
        else if (degrees == 270)
        {
            filternum6(s,image);
            filternum6(s,image);
            filternum6(s,image);
        }
        else
            cout << "invalid input" << "\n";
    }
}