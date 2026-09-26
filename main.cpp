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
      //  else if (name=="Filter4") task4(picture,s);
        else if (name=="Filter5") task5(picture,s);
        // else if (name=="Filter6") task6(picture,s);
        else if (name=="Filter7") task7(picture,s);
        // else if (name=="Filter8") task8(picture,s);
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
