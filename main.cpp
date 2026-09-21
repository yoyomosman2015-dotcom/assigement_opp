#include "Image_Class.h"
using namespace std;
void task1(Image filter1) {
    for (int i = 0; i < filter1.width; i++) {
        for (int j = 0; j < filter1.height; j++) {
            int grayscale=0;
            for (int k = 0; k < filter1.channels; k++) {
                grayscale+=filter1(i, j, k) ;
            }
            filter1(i,j,0)=grayscale;
            filter1(i,j,1)=grayscale;
            filter1(i,j,2)=grayscale;
        }
    }
    filter1.saveImage("test.jpg");
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
