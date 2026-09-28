#include "Image_Class.h"
using namespace std;

void task1(Image filter1,string save_name) {
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
    filter1.saveImage(save_name);
    cout<<"Filter1 is done"<<endl;
}

void task2(Image filter2,string save_name) {
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
    filter2.saveImage(save_name);
    cout<<"Filter2 is done"<<endl;
}

void task3(Image filter3,string save_name) {
    for (int x = 0; x < filter3.width; x++) {
        for (int y = 0; y < filter3.height; y++) {
            for (int z = 0; z < filter3.channels; z++) {
                filter3(x, y, z) = 255 - filter3(x, y, z);
            }
        }
    }
    filter3.saveImage(save_name);
    cout<<"Filter is done"<<endl;
}
void task4(Image image) {
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
void task5(Image filter5,string save_name) {
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
    helper.saveImage(save_name);
    cout<<"Filter5 is done"<<endl;
}
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
    image.saveImage("good.jpg");
}
void task7(Image filter7,string save_name) {
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
    filter7.saveImage(save_name);
    cout<<"Filter7 is done"<<endl;
}
void task8(Image image) {
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
bool
isValidFilename(const std::string& filename) {
    const char* extension = strrchr(filename.c_str(), '.');
    if (extension == nullptr) {
        cout << "Invalid filename: " << filename << endl;
       return false;
    }
return true;
}
// part of saving

int main() {
    cout<<"please enter your filename"<<endl;
    string s;
    cin>>s;
     bool vaild = isValidFilename(s);
    while (!vaild) {
        cin>>s;
        vaild=isValidFilename(s);
    }
    Image picture(s);
    while (true) {
    bool save=false;
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
        string name; cin>>name;
        if (name=="Load") {
           string surname;
            if (!save) {
                cout<<"are you want to save or not"<<endl;
                string save_name; cin>>save_name;
                if (save_name=="Save") {
                    cout<<"Do you want to save these changes to the original file? input 0";
                    int x; cin>>x;
                    if (x==1) {
                        string isname;
                        cin>>isname;

                        bool vaild1 = isValidFilename(isname);
                        while (!vaild1) {
                            cin>>isname;
                            vaild1=isValidFilename(isname);
                        }
                        save = picture.saveImage(isname);
                    }
                    else  save=picture.saveImage(s);
                    cout<<"saving is completed"<<endl;
                }
                }
            cout<<"the name of loaded filter";
            cin>>s;
            isValidFilename(s);
            Image picture(s);
            cout<<"load is completed"<<endl;
        }
        else if (name=="Filter1") task1(picture,s);
        else if (name=="Filter2") task2(picture,s);
        else if (name=="Filter3") task3(picture,s);
        else if (name=="Filter4") task4(picture);
        else if (name=="Filter5") task5(picture,s);
         else if (name=="Filter6") {
             cout << "How many degrees do you want to rotate? (90, 180, 270)" << "\n";
             int degrees;
             cin >> degrees;
             if (degrees == 90)
                 filternum6(s,picture);
             else if (degrees == 180)
             {
                 filternum6(s,picture);
                 filternum6(s,picture);
             }
             else if (degrees == 270)
             {
                 filternum6(s,picture);
                 filternum6(s,picture);
                 filternum6(s,picture);
             }
             else
                 cout << "invalid input" << "\n";
         }
        else if (name=="Filter7") task7(picture,s);
         else if (name=="Filter8") task8(picture);
        else if (name=="Save") {
            cout<<"Do you want to save these changes to the original file? input 0";
           int x; cin>>x;
            if (x==1) {
             string isname;
                cin>>isname;

                bool vaild1 = isValidFilename(isname);
                while (!vaild1) {
                    cin>>isname;
                    vaild1=isValidFilename(isname);
                }
               save = picture.saveImage(isname);
            }
           else  save=picture.saveImage(s);
            cout<<"saving is completeted"<<endl;
        }
        // Exit
        else if ("Exit"==name) {
            cout<<save<<endl;
            if (!save) {
                cout<<"The program alerts the user and asks if they want to save before closing it completely.";
                string save_of_exit; cin>>save_of_exit;
                if (save_of_exit=="exit") return 0;
                else{
                    int x; cin>>x;
                    if (x==1) {
                        string isname; cin>>isname;
                        bool vaild1 = isValidFilename(isname);
                        while (!vaild1) {
                            cin>>isname;
                            vaild1=isValidFilename(isname);
                        }
                        save = picture.saveImage(isname);
                    }
                    else  save=picture.saveImage(s);
                }
            }
            else return 0;
        }
    }
    }