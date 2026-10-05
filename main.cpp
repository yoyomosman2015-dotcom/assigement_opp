#include "Image_Class.h"
#include <cmath>
#include <vector>
using namespace std;
Image task8(Image &image,int width,int height ) {
    if (width<=0) {width=0;}
    if (height<=0) {height=0;}
    Image image2(width,height);
    for (int i=0;i<image2.width;i++) {
        for (int j=0;j<image2.height;j++) {
            for (int k=0;k<3;k++) {
                image2(i,j,k)=image(i*image.width/image2.width,j*image.height/image2.height,k);
            }
        }
    }
    image2.saveImage("test.jpg");
    cout<<"Filter8 is done"<<endl;
    return image2;
}
Image Filter12(Image &image) {
    vector<vector<vector<int>>>pre(image.height,vector<vector<int>>(image.width+1,vector<int>(3,0)));
    for (int j=0;j<image.height;j++) {
        for (int i=1;i<=image.width;i++) {
            for (int k=0;k<3;k++) {
                pre[j][i][k]=image(i-1,j,k)+pre[j][i-1][k];

            }
        }
    }
    int r;
    do {
        cout << "Please enter the blur radius value : ";
        cin >> r;
        bool check=r<0 || image.width<=2*r || image.height<=2*r;
        if (check) cout << "error!!!\n";
    }while (r<0 || image.width<=2*r || image.height<=2*r);

    Image image2(image.width-2*r, image.height-2*r);
    for (int i=r; i<image.width-r; i++) {
        for (int j=r; j<image.height-r; j++) {
            int G=0,R=0,B=0;
            for (int l=j-r; l<=j+r; l++) {
                R+=pre[l][i+r+1][0]-pre[l][i-r][0];
                G+=pre[l][i+r+1][1]-pre[l][i-r][1];
                B+=pre[l][i+r+1][2]-pre[l][i-r][2];
            }
            image2(i-r,j-r,0)=R/((2*r+1)*(2*r+1));
            image2(i-r,j-r,1)=G/((2*r+1)*(2*r+1));
            image2(i-r,j-r,2)=B/((2*r+1)*(2*r+1));
        }
    }
    image=task8(image2,image.width,image.height);
    image.saveImage("test.jpg");
    return image;
}

Image Filter16(Image &picture) {
    for (int i=0;i<picture.width;i++) {
        for (int j=0;j<picture.height;j++) {
            int R=picture(i,j,0);
            int G=picture(i,j,1);
            int B=picture(i,j,2);
            picture(i,j,0)=255;
            int aveg=(R+G+B)/3;
            picture(i,j,1) = 255-aveg;
            picture(i,j,2) = 255-aveg;
        }
    }
    picture.saveImage("test.jpg");
    return picture;
}

int main() {


}
