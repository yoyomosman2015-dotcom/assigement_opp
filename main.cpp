#include "Image_Class.h"
using namespace std;

void task1(Image filter1) {
    for (int i = 0; i < filter1.width; i++) {
        for (int j = 0; j < filter1.height; j++) {
            int grayscale=0;
            for (int k = 0; k < filter1.channels; k++) {
                grayscale += filter1(i, j, k) ;
            }
            grayscale /= 3;
            filter1(i,j,0)=grayscale;
            filter1(i,j,1)=grayscale;
            filter1(i,j,2)=grayscale;
        }
    }
    filter1.saveImage("test.jpg");
}

void task2(Image filter2) {
    for (int x = 0; x < filter2.width; x++) {
        for (int y = 0; y < filter2.height; y++) {
            int avg = 0;
            avg += (filter2(x, y, 0) + filter2(x, y, 1) + filter2(x, y, 2))/3;
            if (avg > 127) avg = 255;
            else avg = 0;
            for (int z = 0; z < 3; z++) {
                filter2(x, y, z) = avg;
            }
        }
    }
    filter2.saveImage("test.jpg");
}

void task3(Image filter3) {
    for (int x = 0; x < filter3.width; x++) {
        for (int y = 0; y < filter3.height; y++) {
            for (int z = 0; z < filter3.channels; z++) {
                filter3(x, y, z) = 255 - filter3(x, y, z);
            }
        }
    }
    filter3.saveImage("test.jpg");
}

void task5(Image filter5) {
    int x= filter5.width-1;
    int y= filter5.height-1;
    Image helper=filter5;
    string s; cin>>s;
    helper.saveImage("test1.jpg");
    if (s=="horizantel") {
        for (int i = 0; i < filter5.width; i++) {
            for (int j = 0; j < filter5.height; j++) {
                helper(x-i,j,0)=filter5(i,j,0);
                helper(x-i,j,1)=filter5(i,j,1);
                helper(x-i,j,2)=filter5(i,j,2);
            }
        }
    }
    else {
        for (int i = 0; i < filter5.width; i++) {
            for (int j = 0; j < filter5.height; j++) {
                helper(i,y-j,0)=filter5(i,j,0);
                helper(i,y-j,1)=filter5(i,j,1);
                helper(i,y-j,2)=filter5(i,j,2);
            }
        }
    }
    helper.saveImage("test.jpg");
}

void task7(Image filter7) {
    int lightening;
    cout << "lightening = ";
    cin >> lightening;
    for (int x = 0; x < filter7.width; x++) {
        for (int y = 0; y < filter7.height; y++) {
            for (int z = 0; z < 3; z++) {
                if ((filter7(x, y, z) + lightening) > 255) filter7(x, y,z) = 255;
                else if ((filter7(x, y, z) + lightening) < 0) filter7(x,y,z) = 0;
                else filter7(x, y, z) += lightening;
            }
        }
    }
    filter7.saveImage("test.jpg");
}

void Filter4(Image image) {
    enum entype{ Fancy=1,Simple};
    cout << "What is the type of image? { Fancy=1,Simple=2}" << endl;
    int y; cin >> y;
    entype type=entype(y);
    Image image2(image.width+42,image.height+42);
    enum encolour { White=1,Black,Red,Green,Blue ,yellow ,Cyan,Pink,Orange,Purple,Gray,Navy_Blue};
    encolour colour;
    cout << "Write the colour number of the frame \n";
    cout << "{ White=1,Black=2,Red=3,Green=4,Blue=5 ,yellow=6 ,Cyan=7,Pink=8,Orange=9,Purple=10,Gray=11,Navy Blue=12}\n";
    int x; cin >> x;
    colour=encolour(x);
    int R,G,B;
    if (colour== encolour::White) { R=255;G=255;B=255;}
    else if (colour== encolour::Black) { R=0;G=0;B=0;}
    else if (colour== encolour::Red) { R=255;G=0;B=0;}
    else if (colour== encolour::Green) { R=0;G=255;B=0;}
    else if (colour== encolour::Blue ) { R=0;G=0;B=255;}
    else if (colour== encolour::yellow) { R=255;G=255;B=0;}
    else if (colour== encolour::Cyan) { R=0;G=255;B=255;}
    else if (colour== encolour::Pink) { R=255;G=0;B=255;}
    else if (colour== encolour::Orange) { R=255;G=165;B=0;}
    else if (colour== encolour::Purple) { R=128;G=0;B=128;}
    else if (colour== encolour::Gray) { R=128;G=128;B=128;}
    else if (colour== encolour::Navy_Blue) { R=0;G=0;B=128;}
    for (int i=0;i<image2.width;i++) {
        for (int j=0;j<image2.height;j++) {
            image2(i,j,0)=R;
            image2(i,j,1)=G;
            image2(i,j,2)=B;
        }
    }
    for (int i=21;i<image2.width-20;i++) {
        for (int j=21;j<image2.height-20;j++) {
            image2(i,j,0)=image(i-21,j-21,0);
            image2(i,j,1)=image(i-21,j-21,1);
            image2(i,j,2)=image(i-21,j-21,2);
        }
    }
    if (type==entype::Fancy) {
        for (int i=22;i<image2.width-22;i++) {
            int y=image2.height-22;
            if (i>40) y=41;
            for (int j=22;j<y;j++) {
                if ( j%2 && i%2) continue;
                image2(i,j,0)=255;
                image2(i,j,1)=255;
                image2(i,j,2)=255;
                image2(image2.width-i,image2.height-j,0)=255;
                image2(image2.width-i,image2.height-j,1)=255;
                image2(image2.width-i,image2.height-j,2)=255;
            }
        }
    }
    image2.saveImage("test.jpg");
}
int main() {
    Image image("luffy.jpg");
    Filter4(image);
}
// لوقت الحاجة 😜
/*
void task15(Image filter15) {
    int deg;
    cout << "purple deg = ";
    cin >> deg;
    for (int x = 0; x < filter15.width; x++) {
        for (int y = 0; y < filter15.height; y++) {
            for (int z = 0; z < 3; z+=2) {
                if (filter15(x, y, z) + deg > 255) {
                    filter15(x, y, z) = 255;
                }
                else if (filter15(x, y, z) + deg < 0) {
                    filter15(x, y, z) = 0;
                }
                else {
                    filter15(x, y, z) += deg;
                }
            }
        }
    }
    filter15.saveImage("test.jpg");
}
*/

int main() {
    cout<<"please load a file to work on it"<<endl;
    string filename; cin>>filename;
    Image image(filename);

    string filtername; cin>>filtername;
    cout<<filtername<<endl;
while (true) {
    if (filtername=="Grayscale Conversion") task1(image);
    else if (filtername=="Flip Image") task5(image);
    else if (filtername=="Save the image") {
        string savefilename; cin>>savefilename;
        if (savefilename=="Save the image")
            else
    }
    else if (filtername=="Exit") return 0;
}
}
