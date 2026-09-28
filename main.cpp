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
void filter8(Image image) {
    int width,height;
    cout << "Enter width height\n";
    cin >> width >> height;
    if (width<=0) {width=0;}
    if (height<=0) {height=0;}
    Image image2(width,height);
    for (int i=0;i<image2.width;i++) {
        for (int j=0;j<image2.height;j++) {
            for (int k=0;k<3;k++) {
                image2(i,j,k)=image(i*image.width/image2.width,j*image.height/image2.height,k);
            }
        }
        image2.saveImage("test.jpg");
    }
}

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
