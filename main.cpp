/*
name                                     id                           filters
youssef mohamed osman                    20250791                      1,5,9,13,17
Abdelrahman Haithem Mostafa Said         20250378                      4,8,12,16,18
Nabil mohamed bahget mohamed             20250699                       6,10,11,14
Youssef Tarek Moawad Eid                 20250773                      2,3,7,15
*/
#include "Image_Class.h"
#include<vector>
using namespace std;
int  DetectIfSaveOrNOt=0;
Image picture;


bool extension(string &s) {
    while (s.size()<5 ) {
        cout<<"invalid input : "<<s<<endl;
        return false;
    }
    while (!((s[s.size()-1]=='g' && s[s.size()-2]=='p' && s[s.size()-3]=='j' && s[s.size()-4]=='.') || (s[s.size()-1]=='p' && s[s.size()-2]=='m' && s[s.size()-3]=='b' && s[s.size()-4]=='.') || (s[s.size()-1]=='g' && s[s.size()-2]=='n' && s[s.size()-3]=='p' && s[s.size()-4]=='.') || (s[s.size()-1]=='a' && s[s.size()-2]=='g' && s[s.size()-3]=='t' && s[s.size()-4]=='.')))
    {
        cout<<"invalid input : "<<s<<endl;
        return false;
    }
    return true;
}

Image task1(Image &filter1) {
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

Image task2(Image &filter2) {
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

Image task3(Image &filter3) {
    for (int x = 0; x < filter3.width; x++) {
        for (int y = 0; y < filter3.height; y++) {
            for (int z = 0; z < filter3.channels; z++) {
                filter3(x, y, z) = 255 - filter3(x, y, z);
            }
        }
    }
    filter3.saveImage("test.jpg");
    cout<<"Filter3 is done"<<endl;
    return filter3;
}

Image task4(Image &image) {
    cout << "Enter the Thickness of the frame : ";
    int size; cin >> size;
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
        else {
            cout << "Invalid\n";
        }
    for (int i=0;i<image.width;i++) {
        int y=image.height;
        if (i>size) y=size;
        for (int j=0;j<y;j++) {
            image(i,j,0)=R;
            image(i,j,1)=G;
            image(i,j,2)=B;
            image(image.width-i-1,image.height-j-1,0)=R;
            image(image.width-i-1,image.height-j-1,1)=G;
            image(image.width-i-1,image.height-j-1,2)=B;
        }
    }
    int start=0;
    if (type==entype::Fancy) {
        for (int i=size/3;i<image.width-size/3;i++) {
            int y=image.height-(size/3);
            if (i>size-size/3) y=size-size/3;
            for (int j=size/3;j<y;j++) {
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
    cout<<"Filter4 is done"<<endl;
    return image;
}

Image task5(Image &filter5) {
    int x= filter5.width-1;
    int y= filter5.height-1;
    Image helper=filter5;
    cout <<"horizantel == h or vertical == v"<<endl;
    string s; cin>>s;
    helper.saveImage("test1.jpg");
    if (s=="h") {
        for (int i = 0; i < filter5.width; i++) {
            for (int j = 0; j < filter5.height; j++) {
                helper(x-i,j,0)=filter5(i,j,0);
                helper(x-i,j,1)=filter5(i,j,1);
                helper(x-i,j,2)=filter5(i,j,2);
            }
        }
    }
    else if (s=="v"){

        for (int i = 0; i < filter5.width; i++) {
            for (int j = 0; j < filter5.height; j++) {
                helper(i,y-j,0)=filter5(i,j,0);
                helper(i,y-j,1)=filter5(i,j,1);
                helper(i,y-j,2)=filter5(i,j,2);
            }
        }
    }
    else cout<< "Invalid!\n";
    helper.saveImage("test.jpg");
    cout<<"Filter5 is done"<<endl;
    return helper;
}

Image filternum6(Image &image)
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
    cout<<"Filter6 is done"<<endl;
    return image;
}

Image task7(Image &filter7) {
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

Image task8(Image &image,int width,int height) {
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

Image task9(Image &image) {
    cout << "Enter merge image : ";
    string mergename; cin >> mergename;
    extension(mergename);
    Image image2(mergename);
    cout<<"to bigger : b , , to smaller : a : ";
    char c; cin >> c;
    int width, height;
    if (c=='b')  width=max(image.width,image2.width),height=max(image.height,image2.height);
    else  width=min(image.width,image2.width),height=min(image.height,image2.height);
    Image image4(width,height);
    Image image5(width,height);
    if (image.width==width && image.height==height) image4=image;
    else image4=task8(image,width,height);

    if (image2.width==width && image2.height==height) image5=image2;
    else image5=task8(image,width,height);

    Image image3(width,height);
    for (int i=0;i<width;i++) {
        for (int j=0;j<height;j++) {
            for (int k=0;k<3;k++) {
                int average=(image4(i,j,k)+image5(i,j,k))/2 ;
                image3(i,j,k)=average;
            }
        }
    }
    image3.saveImage("test.jpg");
    cout<<"Filter9 is done"<<endl;
    return image3;
}

Image filter10(Image& image)
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
    newOne.saveImage("test.jpg");
    cout<<"Filter10 is done"<<endl;
    return newOne;
}

Image filter11(Image& image)
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
             the_crop.saveImage("test.jpg");
                cout<<"Filter11 is done"<<endl;
                return the_crop;

}

Image filter12(Image &image) {
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
    cout<<"Filter12 is done"<<endl;
    return image;
}

Image task13(Image &image) {
    for (int i=0;i<image.width;i++) {
        for (int j=0;j<image.height;j++) {
            for (int k=0;k<2;k++) {
                int avg=image(i,j,k)+30;
                if (avg>255) avg=255;
                image(i,j,k)=avg;
            }
            int avg=image(i,j,2)-30;
            if (avg<0) avg=0;
            image(i,j,2)=avg;

        }
    }
    image.saveImage("test.jpg");
    cout<<"Filter13 is done"<<endl;
    return image;
}

Image filternum14(Image& image){
    for (int i=0;i<image.width;i++)
        for (int j=0;j<image.height;j++){
            if (j%2!=0) continue;
            else for (int c=0;c<3;c++)
            {
                image.setPixel(i,j,c,(image.getPixel(i,j,c)*0.6));
            }
        }
    image.saveImage("test.jpg");
    cout<<"Filter14 is done"<<endl;
    return image;
}

Image task15(Image filter15) {
    for (int x = 0; x < filter15.width; x++) {
        for (int y = 0; y < filter15.height; y++) {
            for (int z = 0; z < 3; z++) {
                if (filter15(x, y, z) - 40 < 0) filter15(x, y, z) = 0;
                else filter15(x, y, z) -= 40;
            }
            if (filter15(x, y, 0) *1.5 > 255) filter15(x, y, 0) = 255;
            else filter15(x, y, 0) = int(filter15(x, y, 0) * 1.5);
        }
    }
    filter15.saveImage("test.jpg");
    cout<<"Filter15 is done"<<endl;
    return filter15;
}

Image filter16(Image &picture) {
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
    cout<<"Filter16 is done"<<endl;
    return picture;
}

Image task17(Image &image, int skew) {
    if (skew <= -90 || skew >= 90) {
        cout << "Angle must be between -89 and 89\n";
        return image;
    }

    double angleRadians = skew * (3.14159265358979323846 / 180.0);
    double t = tan(angleRadians);

    int totalShift = (int)ceil(fabs(t) * (image.height - 1));   // largest row shift
    Image out(image.width + totalShift, image.height);          // wide enough for ALL of the image

    // white background (use 0 for black)
    for (int i = 0; i < out.width; i++)
        for (int j = 0; j < out.height; j++)
            for (int k = 0; k < 3; k++)
                out(i, j, k) = 255;

    for (int j = 0; j < image.height; j++) {
        int shift;
        if (t >= 0) shift = (int)round(t * (image.height - 1 - j));  // top row moves most -> leans right
        else        shift = (int)round(-t * j);                      // negative angle -> leans left

        for (int i = 0; i < image.width; i++)
            for (int k = 0; k < 3; k++)
                out(i + shift, j, k) = image(i, j, k);               // i + shift <= width-1 + totalShift, always inside out
    }
    out.saveImage("test.jpg");
    cout << "filter17 is done" << endl;
    return out;
}

Image filter18( Image &picture) {
    Image image2(picture.width,picture.height);
    for (int i=0;i<picture.width;i++) {
        for (int j=0;j<picture.height;j++) {
            vector<int>intensityCount(21,0),sumR(21,0),sumG(21,0),sumB(21,0);
            for (int h=i-4;h<i+4;h++) {
                for (int v=j-4;v<j+4;v++) {
                    if (h<0 || h>=picture.width || v<0 || v>=picture.height) continue;
                    int aveg=(picture(h,v,0)+picture(h,v,1)+picture(h,v,2))/3;
                    int bin=(aveg*20)/255;
                    intensityCount[bin]++;
                    sumR[bin]+=picture(h,v,0);
                    sumG[bin]+=picture(h,v,1);
                    sumB[bin]+=picture(h,v,2);
                }
            }
            int maxCount=0,maxbin=0;;
            for (int g=0;g<=20;g++)
            {
                if (maxCount<=intensityCount[g]) {
                    maxCount=intensityCount[g];
                    maxbin=g;
                }
            }
            if (intensityCount[maxbin]>0) {
                image2(i,j,0)=sumR[maxbin]/intensityCount[maxbin];
                image2(i,j,1)=sumG[maxbin]/intensityCount[maxbin];
                image2(i,j,2)=sumB[maxbin]/intensityCount[maxbin];
            }
            else {
                image2(i,j,0)=picture(i,j,0);
                image2(i,j,1)=picture(i,j,1);
                image2(i,j,2)=picture(i,j,2);
            }
        }
    }
    image2.saveImage("test.jpg");
    cout<<"Filter18 is done"<<endl;
    return image2;
}

Image loadAFile(string &s) {
    cout<<"please enter photo:"<<endl;
    cin >>s;
    while (!extension(s)) {
        cin>>s;
    }
    Image image(s);
    return image;
}

void List() {
    cout << "*****************<<List>>******************\n";
    cout<<"Load a new image == ld"<<endl;
    cout<<"Filter 1: Grayscale Conversion"<<endl;
    cout<<"Filter 2: Black and White"<<endl;
    cout<<"Filter 3: Invert Image"<<endl;
    cout<<"Filter 4: Adding a Frame to the Picture"<<endl;
    cout<<"Filter 5: Flip Image"<<endl;
    cout<<"Filter 6: Rotate Image"<<endl;
    cout<<"Filter 7: Darken and Lighten Image"<<endl;
    cout<<"Filter 8: Resizing Images"<<endl;
    cout<<"Filter 10: Detect Image Edges"<<endl;
    cout<<"Filter 11: Crop Images"<<endl;
    cout<<"Filter 12: Blur Images"<<endl;
    cout<<"Filter 13: natural sunlight"<<endl;
    cout<<"Filter 14 : old Den Den Mushi (televisions)"<<endl;
    cout<<"Filter 15: Purple at Night Image"<<endl;
    cout<<"Filter 16: infrared photography"<<endl;
    cout<<"Filter 17: Image Skewing Filte"<<endl;
    cout<<"Filter 18: Oil painting i"<<endl;
    cout<<"Save the image == s"<<endl;
    cout<<"Exit == ex"<<endl;
    cout << "***********************************\n";
    cout <<"your choice is: ";
}

void saveImage(Image &picture,string &s) {
    cout<<"Do you want to save these changes to the original file? \"No = 0 or Yes = 1\"";
    int x; cin>>x;
    if (x==0) {
        string isname;
        cout << "New file name: " << '\n';
        cin>>isname;
        while (!extension(isname)) {
            cin>>isname;
        }
        picture.saveImage(isname);
        cout<<"saving is completeted"<<endl;
    }
    else {
        if (picture.saveImage(s)){
            cout<<"saving is completeted"<<endl;
        }
        else cout << "error!\n";
    }
    DetectIfSaveOrNOt=0;
}

void load(string &s) {
    if (DetectIfSaveOrNOt>0) {
        cout<<"Save == 1 , Not Save == 0"<<endl;
        int x; cin>>x;
        if (x==1) saveImage(picture,s);
    }
    picture = loadAFile(s);
    cout<<"load is completed"<<endl;
}

void exiting(string &s) {
    if (DetectIfSaveOrNOt==0) {
        return;
    }
    else {
        cout<<"warning you dont save if you want to save press=1 else 0"<<endl;
        int y; cin>>y;
        if (y==1)  saveImage(picture,s);
        return ;
    }
}

int main() {
    string s;
    loadAFile(s);
    picture.loadNewImage(s);
    while (true) {
        List();
        string name;
        cin>>name;
        if (name=="ld") {
            load(s);
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
            cout<<"Filter6 is done"<<endl;
            DetectIfSaveOrNOt++;
         }
        else if (name=="7") {
            picture = task7(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="8") {
            int width,height;
            cout << "Enter width height\n";
            cin >> width >> height;
            picture  = task8(picture,width,height);
            DetectIfSaveOrNOt++;
        }
        else if (name=="9") {
            picture = task9(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="10") {
            picture = filter10(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="11") {
            picture = filter11(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="12") {
            picture = filter12(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="13") {
            picture = task13(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="14") {
            picture = filternum14(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="15") {
            picture = task15(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="16") {
            picture = filter16(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="17") {
            cout<<"skew: ";
            int skew; cin>>skew;
            if (skew>12) skew-=12;
            picture = task17(picture,skew);
        }
        else if (name=="18") {
            picture = filter18(picture);
            DetectIfSaveOrNOt++;
        }
        else if (name=="s") {
            saveImage(picture,s);
        }
        else if ("ex"==name) {
            exiting(s);
            return 0;
        }
        else cout << "InValid!\n";
    }
}
