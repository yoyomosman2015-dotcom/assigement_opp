#include "Image_Class.h"
using namespace std;
int  DetectIfSaveOrNOt=0;
Image task1(Image filter1) {
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
    cout<<"Filter1 is done"<<endl;
    return filter1;
}

Image task2(Image filter2) {
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
    cout<<"Filter2 is done"<<endl;
    return filter2;
}

Image task3(Image filter3) {
    for (int x = 0; x < filter3.width; x++) {
        for (int y = 0; y < filter3.height; y++) {
            for (int z = 0; z < filter3.channels; z++) {
                filter3(x, y, z) = 255 - filter3(x, y, z);
            }
        }
    }
    filter3.saveImage("test.jpg");
    cout<<"Filter is done"<<endl;
    return filter3;
}

Image task4(Image image) {
    enum entype{ Fancy=1,Simple};
    cout << "What is the type of image? { Fancy=1,Simple=2}" << endl;
    int y; cin >> y;
    entype type=entype(y);
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
    for (int i=0;i<image.width;i++) {
        int y=image.height;
        if (i>20) y=21;
        for (int j=0;j<y;j++) {
            image(i,j,0)=R;
            image(i,j,1)=G;
            image(i,j,2)=B;
            image(image.width-i-1,image.height-j-1,0)=R;
            image(image.width-i-1,image.height-j-1,1)=G;
            image(image.width-i-1,image.height-j-1,2)=B;
        }
    }
    if (type==entype::Fancy) {
        for (int i=0;i<image.width;i++) {
            int y=image.height-20;
            if (i>20) y=21;
            for (int j=0;j<y;j+=7) {
                    image(i,j,0)=255-R;
                    image(i,j,1)=255-G;
                    image(i,j,2)=255-B;
                    image(image.width-i-1,image.height-j-1,0)=255-R;
                    image(image.width-i-1,image.height-j-1,1)=255-G;
                    image(image.width-i-1,image.height-j-1,2)=255-B;
                }

        }
     }
    image.saveImage("test.jpg");
    return image;
}

Image task5(Image filter5) {
    int x= filter5.width-1;
    int y= filter5.height-1;
    Image helper=filter5;
    cout <<"do you want it horizantel press 0 or vertical press 1"<<endl;
    string s; cin>>s;
    helper.saveImage("test1.jpg");
    if (s=="0") {
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
    cout<<"Filter5 is done"<<endl;
    return helper;
}

Image filternum6(Image& image)
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
    image.saveImage("test.jpg");
    return image;
}

Image task7(Image filter7) {
    double lightening;
    cout << "lightening % (-,+)= ";
    cin >> lightening;
    lightening = lightening * 0.01;
    for (int x = 0; x < filter7.width; x++) {
        for (int y = 0; y < filter7.height; y++) {
            for (int z = 0; z < 3; z++) {
                if (filter7(x,y,z)+filter7(x,y,z)*lightening > 255) {
                    filter7(x,y,z)=255;
                }
                else if (filter7(x,y,z)+filter7(x,y,z)*lightening < 0) {
                    filter7(x,y,z)=0;
                }
                else {
                    filter7(x,y,z)+=filter7(x,y,z)*lightening;
                }
            }
        }
    }
    filter7.saveImage("test.jpg");
    cout<<"Filter7 is done"<<endl;
    return filter7;
}

Image task8(Image image) {
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
    }
    image2.saveImage("test.jpg");
    return image2;
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
bool isValidFilename(const std::string& filename) {
    const char* extension = strrchr(filename.c_str(), '.');
    if (extension == nullptr) {
        cout << "Invalid filename: " << filename << endl;
       return false;
    }
return true;
}
// part of saving
string loadAFile() {
    cout<<"please enter photo:"<<endl;
    string s;
    getline(cin, s);
    while (!isValidFilename(s)) {
        cin>>s;
    }
    Image picture3(s); picture3.saveImage("test.jpg");
    return s;
}
void TheMain() {
    cout<<"Load a new image"<<endl;
    cout<<"Filter 1: Grayscale Conversion"<<endl;
    cout<<"Filter 2: Black and White"<<endl;
    cout<<"Filter 3: Invert Image"<<endl;
    cout<<"Filter 4: Adding a Frame to the Picture"<<endl;
    cout<<"Filter 5: Flip Image"<<endl;
    cout<<"Filter 6: Rotate Image"<<endl;
    cout<<"Filter 7: Darken and Lighten Image"<<endl;
    cout<<"Filter 8: Resizing Images"<<endl;
    cout<<"Save the image"<<endl;
    cout<<"Exit"<<endl;
}
void saveImage(Image &picture,string s) {
    cout<<"Do you want to save these changes to the original file? \"No = 0 or Yes = 1\"";
    int x; cin>>x;
    if (x==0) {
        string isname;
        cout << "New file name: " << '\n';
        cin>>isname;
        while (!isValidFilename(isname)) {
            cin>>isname;
        }
        picture.saveImage(isname);
    }
    else {
        if (picture.saveImage(s)){
            cout<<"saving is completeted"<<endl;
        }
        else cout << "error!\n";
    }
    DetectIfSaveOrNOt=0;
}
int main() {
    string s=loadAFile();
    Image picture(s);
    while (true) {
        TheMain();
        string name;
        cin>>name;
        if (name=="load") {
            if (DetectIfSaveOrNOt>0) {
                cout<<"if you want to save the image press 1"<<endl;
                int x; cin>>x;
                if (x==1) saveImage(picture,s);
            }
            cout<<"the name of loaded filter";
            s = loadAFile();
            cout<<"load is completed"<<endl;
        }
        else if (name=="1") {
            picture = task1(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="2") {
            picture = task2(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="3") {
            picture = task3(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="4") {
            picture = task4(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="5") {
            picture = task5(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="6") {
             cout << "How many degrees do you want to rotate? (90, 180, 270)" << "\n";
             int degrees;
             while (true) {
                 cin >> degrees;
                 if (degrees == 90) {
                     picture = filternum6(picture);
                     break;
                 }
                 else if (degrees == 180)
                 {
                     filternum6(picture);
                     picture = filternum6(picture);
                     break;
                 }
                 else if (degrees == 270)
                 {
                     filternum6(picture);
                     filternum6(picture);
                     picture = filternum6(picture);
                     break;
                 }
                 else{
                     cout << "invalid input" << "\n";
                     cout << "please enter valid input;(90, 180, 270):" << "\n";
                 }
             }
            DetectIfSaveOrNOt++;
         }
        else if (name=="7") {
            picture = task7(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="8") {
            picture  = task8(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="save") {
            saveImage(picture,s);
        }
        else if ("exit"==name) {
            if (DetectIfSaveOrNOt==0) {
                return 0;
            }
            else {
                cout<<"warning you dont save if you want to save press=1"<<endl;
                int y; cin>>y;
                if (y==1)  saveImage(picture,s);
                else return 0;
            }
        }
    }
}