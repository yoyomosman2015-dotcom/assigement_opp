#include <wx/wx.h>
#include <wx/image.h>
#include <wx/spinctrl.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <wx/filename.h>
#include <wx/timer.h>
#include <wx/arrstr.h>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// =====================================================================
//  1. كلاس الربط (Wrapper) عشان كودك القديم يشتغل بدون تعديل
// =====================================================================
// هيكل معاملات الفلاتر
// =====================================================================
//  التعريفات الأساسية وجدول الفلاتر (توضع في أعلى الملف تماماً)
// =====================================================================
const int FILTER_COUNT = 18;
const int MAX_PARAMS = 4;

enum ParamType
{
    PARAM_SLIDER,
    PARAM_CHECK,
    PARAM_CHOICE
};

struct ParamDef
{
    int type;
    const char* nameEn;
    const char* nameAr;
    int minV;
    int maxV;
    int defV;
    const char* choicesEn;
    const char* choicesAr;
};

struct FilterParams
{
    int v[MAX_PARAMS];
    FilterParams() {
        for (int i = 0; i < MAX_PARAMS; i++) v[i] = 0;
    }
};

typedef wxImage (*FilterFunc)(const wxImage& img, const FilterParams& p);

struct FilterDef
{
    const char* nameEn;
    const char* nameAr;
    FilterFunc func;
    int paramCount;
    ParamDef params[MAX_PARAMS];
};

// دي المصفوفة الأساسية اللي كانت بتعمل الإيرور
FilterDef g_filters[FILTER_COUNT];

// =====================================================================
//  كلاس الربط (Wrapper)
// =====================================================================
struct MyImageWrapper {
    int width, height, channels;
    vector<unsigned char> data;

    MyImageWrapper(const wxImage& img) {
        width = img.GetWidth();
        height = img.GetHeight();
        channels = 3;
        int size = width * height * 3;
        data.assign(img.GetData(), img.GetData() + size);
    }

    MyImageWrapper(int w, int h) {
        width = w;
        height = h;
        channels = 3;
        data.assign(w * h * 3, 0);
    }

    unsigned char& operator()(int x, int y, int c) {
        return data[(y * width + x) * 3 + c];
    }

    void saveImage(const string& filename) {}

    wxImage toWxImage() {
        wxImage result(width, height);
        unsigned char* buf = (unsigned char*)malloc(width * height * 3);
        memcpy(buf, data.data(), width * height * 3);
        result.SetData(buf, width, height, false);
        return result;
    }
};
// =====================================================================
//  منطق الفلاتر الـ 5 الأولى (بكودك الأصلي تماماً داخل الـ Wrapper)
// =====================================================================

// --- Task 1: Grayscale (أبيض وأسود عادي) ---
MyImageWrapper task1_original(MyImageWrapper &filter1) {
    for (int i = 0; i < filter1.width; i++) {
        for (int j = 0; j < filter1.height; j++) {
            int grayscale = 0;
            for (int k = 0; k < filter1.channels; k++) {
                grayscale += filter1(i, j, k) ;
            }
            grayscale /= 3;
            filter1(i,j,0) = grayscale;
            filter1(i,j,1) = grayscale;
            filter1(i,j,2) = grayscale;
        }
    }
    filter1.saveImage("test.jpg");
    return filter1;
}

// دالة الربط للـ GUI (فلتر 1)
wxImage apply_task1(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    task1_original(wrapper);
    return wrapper.toWxImage();
}


// --- Task 2: Black & White (ثنائي نقي) ---
MyImageWrapper task2_original(MyImageWrapper &filter2) {
    for (int x = 0; x < filter2.width; x++) {
        for (int y = 0; y < filter2.height; y++) {
            int avg = 0;
            avg += (filter2(x, y, 0) + filter2(x, y, 1) + filter2(x, y, 2)) / 3;
            if (avg > 127) avg = 255;
            else avg = 0;
            for (int z = 0; z < 3; z++) {
                filter2(x, y, z) = avg;
            }
        }
    }
    filter2.saveImage("test.jpg");
    return filter2;
}

// دالة الربط للـ GUI (فلتر 2)
wxImage apply_task2(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    task2_original(wrapper);
    return wrapper.toWxImage();
}


// --- Task 3: Invert (عكس الألوان) ---
MyImageWrapper task3_original(MyImageWrapper &filter3) {
    for (int x = 0; x < filter3.width; x++) {
        for (int y = 0; y < filter3.height; y++) {
            for (int z = 0; z < filter3.channels; z++) {
                filter3(x, y, z) = 255 - filter3(x, y, z); // صححنا الأقواس هنا لتناسب طريقتك
            }
        }
    }
    filter3.saveImage("test.jpg");
    return filter3;
}

// دالة الربط للـ GUI (فلتر 3)
wxImage apply_task3(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    task3_original(wrapper);
    return wrapper.toWxImage();
}


// --- Task 4: Frame (إطار الصورة) ---
// تم تعديل قراءة القيم لتأخذ من المعاملات (p.v) بدلاً من الـ cin عشان تشتغل في الواجهة
MyImageWrapper task4_original(MyImageWrapper &image, const FilterParams& p) {
    int size = (p.v[0] > 0) ? p.v[0] : 10; // سُمك الإطار من شريط الـ GUI
    int type = (p.v[1] > 0) ? p.v[1] : 1;  // نوع الإطار (1: بسيط، 2: مزخرف)
    int colour = (p.v[2] > 0) ? p.v[2] : 2; // اللون الافتراضي (2: أسود مثلاً)

    int R, G, B;
    if (colour == 1)      { R = 255; G = 255; B = 255; }
    else if (colour == 2) { R = 0;   G = 0;   B = 0;   }
    else if (colour == 3) { R = 255; G = 0;   B = 0;   }
    else if (colour == 4) { R = 0;   G = 255; B = 0;   }
    else if (colour == 5) { R = 0;   G = 0;   B = 255; }
    else if (colour == 6) { R = 255; G = 255; B = 0;   }
    else if (colour == 7) { R = 0;   G = 255; B = 255; }
    else if (colour == 8) { R = 255; G = 0;   B = 255; }
    else if (colour == 9) { R = 255; G = 165; B = 0;   }
    else if (colour == 10){ R = 128; G = 0;   B = 128; }
    else if (colour == 11){ R = 128; G = 128; B = 128; }
    else if (colour == 12){ R = 0;   G = 0;   B = 128; }
    else { R = 0; G = 0; B = 0; }

    for (int i = 0; i < image.width; i++) {
        int y_lim = image.height;
        if (i > size) y_lim = size;
        for (int j = 0; j < y_lim; j++) {
            image(i, j, 0) = R;
            image(i, j, 1) = G;
            image(i, j, 2) = B;
            image(image.width - i - 1, image.height - j - 1, 0) = R;
            image(image.width - i - 1, image.height - j - 1, 1) = G;
            image(image.width - i - 1, image.height - j - 1, 2) = B;
        }
    }

    if (type == 1) { // Fancy / المزخرف
        for (int i = size / 3; i < image.width - size / 3; i++) {
            int y_lim = image.height - (size / 3);
            if (i > size - size / 3) y_lim = size - size / 3;
            for (int j = size / 3; j < y_lim; j++) {
                image(i, j, 0) = 255 - R;
                image(i, j, 1) = 255 - G;
                image(i, j, 2) = 255 - B;
                image(image.width - i - 1, image.height - j - 1, 0) = 255 - R;
                image(image.width - i - 1, image.height - j - 1, 1) = 255 - G;
                image(image.width - i - 1, image.height - j - 1, 2) = 255 - B;
            }
        }
    }
    image.saveImage("test.jpg");
    return image;
}

// دالة الربط للـ GUI (فلتر 4)
wxImage apply_task4(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    task4_original(wrapper, p);
    return wrapper.toWxImage();
}


// --- Task 5: Flip (القلب / الانعكاس) ---
MyImageWrapper task5_original(MyImageWrapper &filter5, const FilterParams& p) {
    int x_max = filter5.width - 1;
    int y_max = filter5.height - 1;
    MyImageWrapper helper = filter5;

    // p.v[0] يحدد الاتجاه: 0 لأفقي (h)، 1 رأسي (v)
    string s = (p.v[0] == 1) ? "v" : "h";

    if (s == "h") {
        for (int i = 0; i < filter5.width; i++) {
            for (int j = 0; j < filter5.height; j++) {
                helper(x_max - i, j, 0) = filter5(i, j, 0);
                helper(x_max - i, j, 1) = filter5(i, j, 1);
                helper(x_max - i, j, 2) = filter5(i, j, 2);
            }
        }
    }
    else if (s == "v") {
        for (int i = 0; i < filter5.width; i++) {
            for (int j = 0; j < filter5.height; j++) {
                helper(i, y_max - j, 0) = filter5(i, j, 0);
                helper(i, y_max - j, 1) = filter5(i, j, 1);
                helper(i, y_max - j, 2) = filter5(i, j, 2);
            }
        }
    }
    helper.saveImage("test.jpg");
    return helper;
}

// دالة الربط للـ GUI (فلتر 5)
wxImage apply_task5(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    task5_original(wrapper, p);
    return wrapper.toWxImage();
}
// =====================================================================
//  منطق الفلاتر (من 6 إلى 10) باستخدام الكود الأصلي تماماً
// =====================================================================

// --- Task 6: Rotate (تدوير الصورة) ---
MyImageWrapper task6_original(MyImageWrapper &image) {
    // تم تصحيح طريقة الوصول للمتغيرات لتتناسب مع الـ Wrapper
    MyImageWrapper newIma(image.height, image.width);

    for (int oldcol = 0; oldcol < image.width; oldcol++) {
        for (int oldrow = 0; oldrow < image.height; oldrow++) {
            int newrow = 0, newcol = 0;
            newrow = oldcol;
            newcol = (image.height - 1) - oldrow;

            for (int c = 0; c < image.channels; c++) {
                newIma(newcol, newrow, c) = image(oldcol, oldrow, c);
            }
        }
    }
    image = newIma;
    image.saveImage("test.jpg");
    return image;
}

wxImage apply_task6(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    task6_original(wrapper);
    return wrapper.toWxImage();
}


// --- Task 7: Lightening (تعديل الإضاءة) ---
MyImageWrapper task7_original(MyImageWrapper &filter7, const FilterParams& p) {
    // قراءة النسبة من معامل الـ GUI (افتراضياً 0 أو القيمة المرسلة)
    double lightening = (p.v[0] != 0) ? p.v[0] : 0;
    lightening = lightening * 0.01;

    for (int x = 0; x < filter7.width; x++) {
        for (int y = 0; y < filter7.height; y++) {
            for (int z = 0; z < 3; z++) {
                double val = filter7(x, y, z) + filter7(x, y, z) * lightening;
                if (val > 255) {
                    filter7(x, y, z) = 255;
                }
                else if (val < 0) {
                    filter7(x, y, z) = 0;
                }
                else {
                    filter7(x, y, z) = (unsigned char)val;
                }
            }
        }
    }
    filter7.saveImage("test.jpg");
    return filter7;
}

wxImage apply_task7(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    task7_original(wrapper, p);
    return wrapper.toWxImage();
}


// --- Task 8: Resize (تغيير الحجم) ---
MyImageWrapper task8_original(MyImageWrapper &image, int width, int height) {
    if (width <= 0) { width = 1; }
    if (height <= 0) { height = 1; }

    MyImageWrapper image2(width, height);
    for (int i = 0; i < image2.width; i++) {
        for (int j = 0; j < image2.height; j++) {
            for (int k = 0; k < 3; k++) {
                int srcX = i * image.width / image2.width;
                int srcY = j * image.height / image2.height;
                image2(i, j, k) = image(srcX, srcY, k);
            }
        }
    }
    image2.saveImage("test.jpg");
    return image2;
}

wxImage apply_task8(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    // أخذ الأبعاد الجديدة من المعاملات أو افتراضياً نصف الحجم أو حجم محدد
    int newW = (p.v[0] > 0) ? p.v[0] : img.GetWidth() / 2;
    int newH = (p.v[1] > 0) ? p.v[1] : img.GetHeight() / 2;
    MyImageWrapper result = task8_original(wrapper, newW, newH);
    return result.toWxImage();
}


// --- Task 9: Merge Images (دمج صورتين) ---
MyImageWrapper task9_original(MyImageWrapper &image, MyImageWrapper &image2, char c) {
    int width, height;
    if (c == 'b') {
        width = max(image.width, image2.width);
        height = max(image.height, image2.height);
    } else {
        width = min(image.width, image2.width);
        height = min(image.height, image2.height);
    }

    MyImageWrapper image4 = (image.width == width && image.height == height) ? image : task8_original(image, width, height);
    MyImageWrapper image5 = (image2.width == width && image2.height == height) ? image2 : task8_original(image2, width, height);

    MyImageWrapper image3(width, height);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < 3; k++) {
                int average = (image4(i, j, k) + image5(i, j, k)) / 2;
                image3(i, j, k) = average;
            }
        }
    }
    image3.saveImage("test.jpg");
    return image3;
}

wxImage apply_task9(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    // للتجربة في الواجهة، ندمجها مع نسخة منها أو صورة ثانية
    char mode = (p.v[0] == 1) ? 'b' : 'a';
    MyImageWrapper wrapper2 = wrapper; // كمثال افتراضي للصورة الثانية
    MyImageWrapper result = task9_original(wrapper, wrapper2, mode);
    return result.toWxImage();
}


// --- Task 10: Detect Edges / Black & White Edges (كشف الحواف) ---
MyImageWrapper filter10_original(MyImageWrapper &image) {
    MyImageWrapper grayImg = image;
    for (int x = 0; x < image.width; x++) {
        for (int y = 0; y < image.height; y++) {
            int result = 0;
            for (int z = 0; z < image.channels; z++) {
                result += image(x, y, z);
            }
            result = result / 3;
            if (result > 127) result = 255;
            else result = 0;

            grayImg(x, y, 0) = result;
            grayImg(x, y, 1) = result;
            grayImg(x, y, 2) = result;
        }
    }

    MyImageWrapper newOne = grayImg;
    for (int i = 1; i < newOne.width - 1; i++) {
        for (int j = 1; j < newOne.height - 1; j++) {
            if (grayImg(i, j - 1, 0) == 0 && grayImg(i - 1, j, 0) == 0 &&
                grayImg(i + 1, j, 0) == 0 && grayImg(i, j + 1, 0) == 0) {
                newOne(i, j, 0) = 255;
                newOne(i, j, 1) = 255;
                newOne(i, j, 2) = 255;
            }
        }
    }
    newOne.saveImage("test.jpg");
    return newOne;
}

wxImage apply_task10(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    MyImageWrapper result = filter10_original(wrapper);
    return result.toWxImage();
}
// =====================================================================
//  منطق الفلاتر من 11 إلى 15 (بطريقتك الأصلية + الـ Wrapper للـ GUI)
// =====================================================================

// --- Filter 11: Crop (قص جزء من الصورة) ---
MyImageWrapper filter11_original(MyImageWrapper &image, const FilterParams& p) {
    // أخذ إحداثيات القص والأبعاد من المعاملات في الـ GUI بدلاً من الـ cin
    int x = p.v[0];
    int y = p.v[1];
    int W = p.v[2];
    int H = p.v[3];

    // لو القيم غير مفعلة، نأخذ الصورة كاملة كافتراضي
    if (W <= 0) W = image.width - x;
    if (H <= 0) H = image.height - y;

    // حماية الحدود
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + W > image.width) W = image.width - x;
    if (y + H > image.height) H = image.height - y;

    MyImageWrapper the_crop(W, H);
    for (int i = x; i < x + W; i++) {
        for (int j = y; j < y + H; j++) {
            for (int c = 0; c < 3; c++) {
                the_crop(i - x, j - y, c) = image(i, j, c);
            }
        }
    }
    the_crop.saveImage("test.jpg");
    return the_crop;
}

wxImage apply_filter11(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    MyImageWrapper result = filter11_original(wrapper, p);
    return result.toWxImage();
}


// --- Filter 12: Blur (الزغللة باستخدام المجموع التراكمي Prefix Sums) ---
MyImageWrapper filter12_original(MyImageWrapper &image, const FilterParams& p) {
    int r = p.v[0]; // نصف قطر الـ Blur من الـ GUI
    if (r <= 0) r = 2; // قيمة افتراضية لو مشغلتش من الشريط

    vector<vector<vector<int>>> pre(image.height, vector<vector<int>>(image.width + 1, vector<int>(3, 0)));
    for (int j = 0; j < image.height; j++) {
        for (int i = 1; i <= image.width; i++) {
            for (int k = 0; k < 3; k++) {
                pre[j][i][k] = image(i - 1, j, k) + pre[j][i - 1][k];
            }
        }
    }

    if (image.width <= 2 * r || image.height <= 2 * r) {
        return image; // حماية ضد الأبعاد الصغيرة
    }

    MyImageWrapper image2(image.width - 2 * r, image.height - 2 * r);
    for (int i = r; i < image.width - r; i++) {
        for (int j = r; j < image.height - r; j++) {
            int G = 0, R = 0, B = 0;
            for (int l = j - r; l <= j + r; l++) {
                R += pre[l][i + r + 1][0] - pre[l][i - r][0];
                G += pre[l][i + r + 1][1] - pre[l][i - r][1];
                B += pre[l][i + r + 1][2] - pre[l][i - r][2];
            }
            int area = (2 * r + 1) * (2 * r + 1);
            image2(i - r, j - r, 0) = R / area;
            image2(i - r, j - r, 1) = G / area;
            image2(i - r, j - r, 2) = B / area;
        }
    }

    // استخدام دالة تغيير الحجم لإرجاع الصورة لأبعادها الأصلية
    // (افتراضياً نرجع image2 لو احتجت)
    image2.saveImage("test.jpg");
    return image2;
}

wxImage apply_filter12(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    MyImageWrapper result = filter12_original(wrapper, p);
    return result.toWxImage();
}


// --- Filter 13: Purple / Color Tone Effect ---
MyImageWrapper task13_original(MyImageWrapper &image) {
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            for (int k = 0; k < 2; k++) {
                int avg = image(i, j, k) + 30;
                if (avg > 255) avg = 255;
                image(i, j, k) = avg;
            }
            int avg = image(i, j, 2) - 30;
            if (avg < 0) avg = 0;
            image(i, j, 2) = avg;
        }
    }
    image.saveImage("test.jpg");
    return image;
}

wxImage apply_task13(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    task13_original(wrapper);
    return wrapper.toWxImage();
}


// --- Filter 14: TV Lines / Sunken Effect ---
MyImageWrapper filternum14_original(MyImageWrapper &image) {
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            if (j % 2 != 0) continue;
            else {
                for (int c = 0; c < 3; c++) {
                    image(i, j, c) = (unsigned char)(image(i, j, c) * 0.6);
                }
            }
        }
    }
    image.saveImage("test.jpg");
    return image;
}

wxImage apply_filternum14(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    filternum14_original(wrapper);
    return wrapper.toWxImage();
}


// --- Filter 15: Darken & Contrast Boost ---
MyImageWrapper task15_original(MyImageWrapper filter15) {
    for (int x = 0; x < filter15.width; x++) {
        for (int y = 0; y < filter15.height; y++) {
            for (int z = 0; z < 3; z++) {
                if (filter15(x, y, z) - 40 < 0) filter15(x, y, z) = 0;
                else filter15(x, y, z) -= 40;
            }
            int val = (int)(filter15(x, y, 0) * 1.5);
            if (val > 255) filter15(x, y, 0) = 255;
            else filter15(x, y, 0) = val;
        }
    }
    filter15.saveImage("test.jpg");
    return filter15;
}

wxImage apply_task15(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    MyImageWrapper result = task15_original(wrapper);
    return result.toWxImage();
}
// =====================================================================
//  منطق الفلاتر من 16 إلى 18 (بالطريقة الأصلية المألوفة ليك)
// =====================================================================

// --- Filter 16: Sunlight / Purple-Red Effect ---
MyImageWrapper filter16_original(MyImageWrapper &picture) {
    for (int i = 0; i < picture.width; i++) {
        for (int j = 0; j < picture.height; j++) {
            int R = picture(i, j, 0);
            int G = picture(i, j, 1);
            int B = picture(i, j, 2);

            picture(i, j, 0) = 255;
            int aveg = (R + G + B) / 3;
            picture(i, j, 1) = 255 - aveg;
            picture(i, j, 2) = 255 - aveg;
        }
    }
    picture.saveImage("test.jpg");
    return picture;
}

wxImage apply_filter16(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    filter16_original(wrapper);
    return wrapper.toWxImage();
}


// --- Filter 17: Skew Image (إمالة الصورة بزاوية) ---
MyImageWrapper filter17_original(MyImageWrapper &image, int skew) {
    if (skew <= -90 || skew >= 90) {
        return image;
    }

    double angleRadians = skew * (3.14159265358979323846 / 180.0);
    double t = tan(angleRadians);

    int totalShift = (int)ceil(fabs(t) * (image.height - 1));
    MyImageWrapper out(image.width + totalShift, image.height);

    // خلفية بيضاء
    for (int i = 0; i < out.width; i++) {
        for (int j = 0; j < out.height; j++) {
            out(i, j, 0) = 255;
            out(i, j, 1) = 255;
            out(i, j, 2) = 255;
        }
    }

    for (int j = 0; j < image.height; j++) {
        int shift;
        if (t >= 0) shift = (int)round(t * (image.height - 1 - j));
        else        shift = (int)round(-t * j);

        for (int i = 0; i < image.width; i++) {
            for (int k = 0; k < 3; k++) {
                out(i + shift, j, k) = image(i, j, k);
            }
        }
    }
    out.saveImage("test.jpg");
    return out;
}

wxImage apply_filter17(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    int skewVal = (p.v[0] != 0) ? p.v[0] : 25; // زاوية افتراضية من الواجهة لو مش محددة
    MyImageWrapper result = filter17_original(wrapper, skewVal);
    return result.toWxImage();
}


// --- Filter 18: Oil Painting / Special Blur Filter ---
MyImageWrapper filter18_original(MyImageWrapper &picture) {
    MyImageWrapper image2(picture.width, picture.height);
    for (int i = 0; i < picture.width; i++) {
        for (int j = 0; j < picture.height; j++) {
            vector<int> intensityCount(21, 0), sumR(21, 0), sumG(21, 0), sumB(21, 0);
            for (int h = i - 4; h < i + 4; h++) {
                for (int v = j - 4; v < j + 4; v++) {
                    if (h < 0 || h >= picture.width || v < 0 || v >= picture.height) continue;
                    int aveg = (picture(h, v, 0) + picture(h, v, 1) + picture(h, v, 2)) / 3;
                    int bin = (aveg * 20) / 255;
                    intensityCount[bin]++;
                    sumR[bin] += picture(h, v, 0);
                    sumG[bin] += picture(h, v, 1);
                    sumB[bin] += picture(h, v, 2);
                }
            }
            int maxCount = 0, maxbin = 0;
            for (int g = 0; g <= 20; g++) {
                if (maxCount <= intensityCount[g]) {
                    maxCount = intensityCount[g];
                    maxbin = g;
                }
            }
            if (intensityCount[maxbin] > 0) {
                image2(i, j, 0) = sumR[maxbin] / intensityCount[maxbin];
                image2(i, j, 1) = sumG[maxbin] / intensityCount[maxbin];
                image2(i, j, 2) = sumB[maxbin] / intensityCount[maxbin];
            } else {
                image2(i, j, 0) = picture(i, j, 0);
                image2(i, j, 1) = picture(i, j, 1);
                image2(i, j, 2) = picture(i, j, 2);
            }
        }
    }
    image2.saveImage("test.jpg");
    return image2;
}

wxImage apply_filter18(const wxImage& img, const FilterParams& p) {
    MyImageWrapper wrapper(img);
    MyImageWrapper result = filter18_original(wrapper);
    return result.toWxImage();
}
// =====================================================================
//  القسم 3: جدول الفلاتر
// =====================================================================

void SetFilter(int i, const char* en, const char* ar, FilterFunc func)
{
    g_filters[i].nameEn = en;
    g_filters[i].nameAr = ar;
    g_filters[i].func = func;
    g_filters[i].paramCount = 0;
}

void AddSlider(int i, const char* en, const char* ar, int minV, int maxV, int defV)
{
    FilterDef& f = g_filters[i];
    if (f.paramCount >= MAX_PARAMS)
        return;

    ParamDef& p = f.params[f.paramCount];
    p.type = PARAM_SLIDER;
    p.nameEn = en;
    p.nameAr = ar;
    p.minV = minV;
    p.maxV = maxV;
    p.defV = defV;
    p.choicesEn = "";
    p.choicesAr = "";
    f.paramCount++;
}

void AddCheck(int i, const char* en, const char* ar, int defV)
{
    FilterDef& f = g_filters[i];
    if (f.paramCount >= MAX_PARAMS)
        return;

    ParamDef& p = f.params[f.paramCount];
    p.type = PARAM_CHECK;
    p.nameEn = en;
    p.nameAr = ar;
    p.minV = 0;
    p.maxV = 1;
    p.defV = defV;
    p.choicesEn = "";
    p.choicesAr = "";
    f.paramCount++;
}

void AddChoice(int i, const char* en, const char* ar, const char* choicesEn, const char* choicesAr, int defV)
{
    FilterDef& f = g_filters[i];
    if (f.paramCount >= MAX_PARAMS)
        return;

    ParamDef& p = f.params[f.paramCount];
    p.type = PARAM_CHOICE;
    p.nameEn = en;
    p.nameAr = ar;
    p.minV = 0;
    p.maxV = 0;
    p.defV = defV;
    p.choicesEn = choicesEn;
    p.choicesAr = choicesAr;
    f.paramCount++;
}

void InitFilters()
{
    SetFilter(0, "Filter 1: Grayscale", "فلتر 1: تحويل للرمادي", apply_task1);

    SetFilter(1, "Filter 2: Black & White", "فلتر 2: أبيض وأسود", apply_task2);

    SetFilter(2, "Filter 3: Invert Image", "فلتر 3: عكس الألوان", apply_task3);

    SetFilter(3, "Filter 4: Add Frame", "فلتر 4: إضافة إطار", apply_task4);
    AddSlider(3, "Thickness", "سمك الإطار", 1, 150, 20);
    AddChoice(3, "Type", "النوع", "Fancy|Simple", "مزخرف|بسيط", 0);
    AddChoice(3, "Colour", "اللون",
              "White|Black|Red|Green|Blue|Yellow|Cyan|Pink|Orange|Purple|Gray|Navy Blue",
              "أبيض|أسود|أحمر|أخضر|أزرق|أصفر|سماوي|وردي|برتقالي|بنفسجي|رمادي|كحلي", 0);

    SetFilter(4, "Filter 5: Flip Image", "فلتر 5: قلب الصورة", apply_task5);
    AddChoice(4, "Direction", "الاتجاه", "Horizontal|Vertical", "أفقي|عمودي", 0);

    SetFilter(5, "Filter 6: Rotate Image", "فلتر 6: تدوير الصورة", apply_task6);
    AddChoice(5, "Angle", "الزاوية", "90|180|270", "90|180|270", 0);

    SetFilter(6, "Filter 7: Lighten / Darken", "فلتر 7: الإضاءة والتعتيم", apply_task7);
    AddSlider(6, "Lightening %", "نسبة الإضاءة %", -100, 100, 0);

    SetFilter(7, "Filter 8: Resize Image", "فلتر 8: تغيير الحجم", apply_task8);
    AddSlider(7, "Width %", "نسبة العرض %", 10, 300, 100);
    AddSlider(7, "Height %", "نسبة الطول %", 10, 300, 100);

    SetFilter(8,  "Filter 9: Merge Images",  "فلتر 9: دمج الصور",  apply_task9);
    AddChoice(8, "Merge Size", "حجم الدمج", "Bigger|Smaller", "الأكبر|الأصغر", 0);

    SetFilter(9,  "Filter 10: Edge Detect", "فلتر 10: كشف الحواف", apply_task10);

    SetFilter(10, "Filter 11: Crop Image", "فلتر 11: قص الصورة", apply_filter11);
    AddSlider(10, "X Position %", "موقع X %", 0, 90, 0);
    AddSlider(10, "Y Position %", "موقع Y %", 0, 90, 0);
    AddSlider(10, "Width %", "العرض %", 10, 100, 100);
    AddSlider(10, "Height %", "الطول %", 10, 100, 100);

    SetFilter(11, "Filter 12: Blur Image", "فلتر 12: التغبيش", apply_filter12);
    AddSlider(11, "Blur Radius", "نصف قطر التغبيش", 1, 30, 5);

    SetFilter(12, "Filter 13: Sunlight Adjust", "فلتر 13: ضبط ضوء الشمس", apply_task13);
    SetFilter(13, "Filter 14: Scanlines", "فلتر 14: خطوط المسح", apply_filternum14);

    SetFilter(14, "Filter 15: Purple at Night", "فلتر 15: ليل بنفسجي", apply_task15);

    SetFilter(15, "Filter 16: Reddish Tint", "فلتر 16: فلتر أحمر", apply_filter16);

    SetFilter(16, "Filter 17: Skew Image", "فلتر 17: إمالة الصورة", apply_filter17);
    AddSlider(16, "Skew Angle", "زاوية الإمالة", -80, 80, 0);

    SetFilter(17, "Filter 18: Oil Painting", "فلتر 18: الرسم الزيتي", apply_filter18);
}

// =====================================================================
//  القسم 4: اللغة
// =====================================================================

bool g_arabic = true;   // اللغة الحالية

enum StrId
{
    S_TITLE, S_FILE, S_OPEN, S_SAVE, S_FILTERS, S_APPLY,
    S_HISTORY, S_UNDO, S_REDO, S_RESET, S_CAPACITY, S_STEPS,
    S_SETTINGS, S_DARK_MODE, S_LIGHT_MODE,
    S_ORIGINAL, S_EDITED, S_NO_IMAGE,
    S_OPEN_DLG, S_SAVE_DLG, S_LOAD_FAIL, S_SAVE_FAIL, S_OPEN_FIRST, S_ERROR,
    S_OPTIONS, S_PREVIEW, S_DBL_RESET,
    S_COUNT
};

struct StrPair
{
    const char* en;
    const char* ar;
};

const StrPair g_strings[S_COUNT] = {
    { "Photo Editor",               "محرر الصور" },
    { "File",                       "الملف" },
    { "Open",                       "فتح" },
    { "Save",                       "حفظ" },
    { "Filters",                    "الفلاتر" },
    { "Apply filter",               "تطبيق الفلتر" },
    { "History",                    "السجل" },
    { "Undo",                       "تراجع" },
    { "Redo",                       "إعادة" },
    { "Reset to original",          "العودة للصورة الأصلية" },
    { "History size:",              "حجم السجل:" },
    { "Saved steps",                "الخطوات المحفوظة" },
    { "Settings",                   "الإعدادات" },
    { "Dark mode",                  "الوضع الداكن" },
    { "Light mode",                 "الوضع الفاتح" },
    { "Original",                   "الصورة الأصلية" },
    { "Edited",                     "بعد التعديل" },
    { "Open an image to start",     "افتح صورة للبدء" },
    { "Open image",                 "فتح صورة" },
    { "Save image",                 "حفظ الصورة" },
    { "Could not load this image.", "تعذر تحميل هذه الصورة." },
    { "Could not save the image.",  "تعذر حفظ الصورة." },
    { "Please open an image first.", "من فضلك افتح صورة أولا." },
    { "Error",                      "خطأ" },
    { "Options",                    "الخيارات" },
    { "Preview",                    "معاينة" },
    { "Double-click to reset",      "اضغط مرتين للإرجاع للقيمة الافتراضية" }
};

wxString Tr(int id)
{
    if (g_arabic)
        return wxString::FromUTF8(g_strings[id].ar);
    return wxString::FromUTF8(g_strings[id].en);
}

// =====================================================================
//  القسم 5: الألوان (فاتح / داكن)
// =====================================================================

struct Theme
{
    wxColour bg;        // خلفية النافذة
    wxColour panel;     // خلفية اللوحة الجانبية
    wxColour canvas;    // خلفية بطاقة الصورة
    wxColour field;     // لون الأزرار الثانوية
    wxColour hover;     // لون الأزرار عند المرور
    wxColour hover2;    // لون الأزرار الثانوية عند المرور
    wxColour text;
    wxColour subText;
    wxColour accent;
    wxColour border;
};

Theme g_theme;
bool g_dark = true;

void LoadTheme()
{
    if (g_dark)
    {
        g_theme.bg      = wxColour(24, 26, 33);
        g_theme.panel   = wxColour(33, 36, 46);
        g_theme.canvas  = wxColour(17, 18, 23);
        g_theme.field   = wxColour(46, 50, 64);
        g_theme.hover   = wxColour(130, 116, 245);
        g_theme.hover2  = wxColour(60, 65, 82);
        g_theme.text    = wxColour(235, 237, 245);
        g_theme.subText = wxColour(150, 155, 175);
        g_theme.accent  = wxColour(108, 92, 231);
        g_theme.border  = wxColour(55, 59, 74);
    }
    else
    {
        g_theme.bg      = wxColour(240, 242, 248);
        g_theme.panel   = wxColour(255, 255, 255);
        g_theme.canvas  = wxColour(225, 229, 238);
        g_theme.field   = wxColour(232, 235, 244);
        g_theme.hover   = wxColour(112, 98, 235);
        g_theme.hover2  = wxColour(219, 223, 236);
        g_theme.text    = wxColour(30, 32, 44);
        g_theme.subText = wxColour(105, 110, 130);
        g_theme.accent  = wxColour(92, 76, 220);
        g_theme.border  = wxColour(210, 214, 226);
    }
}

// =====================================================================
//  القسم 6: زر مسطح بتصميم عصري (يتبع الثيم)
// =====================================================================

class FlatButton : public wxPanel
{
public:
    FlatButton(wxWindow* parent, int id, const wxString& caption, bool primary)
        : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize)
    {
        m_caption = caption;
        m_primary = primary;
        m_hover = false;
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        SetMinSize(wxSize(70, 38));
        SetCursor(wxCursor(wxCURSOR_HAND));
    }

    void SetCaption(const wxString& caption)
    {
        m_caption = caption;
        Refresh();
    }

    virtual bool Enable(bool enable = true)
    {
        bool result = wxPanel::Enable(enable);
        Refresh();
        return result;
    }

    void OnPaint(wxPaintEvent& event)
    {
        wxAutoBufferedPaintDC dc(this);
        wxSize size = GetClientSize();

        dc.SetBackground(wxBrush(g_theme.panel));
        dc.Clear();

        wxColour fill;
        wxColour textColor;
        if (m_primary)
        {
            fill = m_hover ? g_theme.hover : g_theme.accent;
            textColor = *wxWHITE;
        }
        else
        {
            fill = m_hover ? g_theme.hover2 : g_theme.field;
            textColor = g_theme.text;
        }
        if (!IsEnabled())
        {
            fill = g_theme.field;
            textColor = g_theme.subText;
        }

        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(fill));
        dc.DrawRoundedRectangle(0, 0, size.x, size.y, 9);

        wxFont font = GetFont();
        font.SetWeight(wxFONTWEIGHT_BOLD);
        dc.SetFont(font);
        dc.SetTextForeground(textColor);
        dc.DrawLabel(m_caption, wxRect(0, 0, size.x, size.y), wxALIGN_CENTER);
    }

    void OnEnter(wxMouseEvent& event)
    {
        m_hover = true;
        Refresh();
        event.Skip();
    }

    void OnLeave(wxMouseEvent& event)
    {
        m_hover = false;
        Refresh();
        event.Skip();
    }

    void OnLeftUp(wxMouseEvent& event)
    {
        wxRect area(GetClientSize());
        if (IsEnabled() && area.Contains(event.GetPosition()))
        {
            wxCommandEvent click(wxEVT_BUTTON, GetId());
            click.SetEventObject(this);
            GetEventHandler()->ProcessEvent(click);
        }
        event.Skip();
    }

private:
    wxString m_caption;
    bool m_primary;
    bool m_hover;

    DECLARE_EVENT_TABLE()
};

wxBEGIN_EVENT_TABLE(FlatButton, wxPanel)
    EVT_PAINT(FlatButton::OnPaint)
    EVT_ENTER_WINDOW(FlatButton::OnEnter)
    EVT_LEAVE_WINDOW(FlatButton::OnLeave)
    EVT_LEFT_UP(FlatButton::OnLeftUp)
wxEND_EVENT_TABLE()

// =====================================================================
//  القسم 7: شريط التمرير (يتبع الثيم)
// =====================================================================

class FlatSlider : public wxPanel
{
public:
    FlatSlider(wxWindow* parent, int id, const wxString& caption,
               int minV, int maxV, int defaultValue)
        : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize,
                  wxTAB_TRAVERSAL | wxFULL_REPAINT_ON_RESIZE)
    {
        m_caption = caption;
        m_min = minV;
        m_max = maxV;
        m_default = defaultValue;
        m_value = defaultValue;
        m_drag = false;
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        SetMinSize(wxSize(120, 66));
        SetCursor(wxCursor(wxCURSOR_HAND));
    }

    int GetValue() const
    {
        return m_value;
    }

    void SetValue(int value)
    {
        m_value = Clamp(value);
        Refresh();
    }

    void OnPaint(wxPaintEvent& event)
    {
        wxAutoBufferedPaintDC pdc(this);
        wxGCDC dc(pdc);
        wxSize size = GetClientSize();

        dc.SetBackground(wxBrush(g_theme.panel));
        dc.Clear();

        wxFont font = GetFont();
        dc.SetFont(font);
        dc.SetTextForeground(g_theme.text);
        dc.DrawText(m_caption, 0, 0);

        wxFont boldFont = font;
        boldFont.SetWeight(wxFONTWEIGHT_BOLD);
        dc.SetFont(boldFont);
        dc.SetTextForeground(g_theme.accent);
        wxString valueText = FormatValue(m_value);
        wxSize valueSize = dc.GetTextExtent(valueText);
        dc.DrawText(valueText, size.x - valueSize.x, 0);

        int trackY = 36;
        int x0 = PAD;
        int x1 = size.x - PAD;

        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(g_theme.border));
        dc.DrawRoundedRectangle(x0, trackY - 2, x1 - x0, 5, 2.5);

        bool bipolar = (m_min < 0 && m_max > 0);
        int origin = bipolar ? 0 : m_min;
        int originX = ValueToX(origin);
        int valueX = ValueToX(m_value);

        int fillX = originX < valueX ? originX : valueX;
        int fillW = valueX > originX ? valueX - originX : originX - valueX;
        if (fillW > 0)
        {
            dc.SetBrush(wxBrush(g_theme.accent));
            dc.DrawRoundedRectangle(fillX, trackY - 2, fillW, 5, 2.5);
        }

        if (bipolar)
        {
            dc.SetPen(wxPen(g_theme.subText, 1));
            dc.DrawLine(originX, trackY - 8, originX, trackY + 9);
        }

        dc.SetPen(wxPen(g_theme.accent, 2));
        dc.SetBrush(wxBrush(*wxWHITE));
        dc.DrawCircle(valueX, trackY, 8);

        dc.SetFont(font);
        dc.SetTextForeground(g_theme.subText);
        int labelY = trackY + 14;

        wxString minText = wxString::Format("%d", m_min);
        wxString maxText = wxString::Format("%d", m_max);
        wxSize maxSize = dc.GetTextExtent(maxText);
        dc.DrawText(minText, 0, labelY);
        dc.DrawText(maxText, size.x - maxSize.x, labelY);

        if (bipolar)
        {
            wxSize zeroSize = dc.GetTextExtent("0");
            dc.DrawText("0", originX - zeroSize.x / 2, labelY);
        }
    }

    void OnLeftDown(wxMouseEvent& event)
    {
        m_drag = true;
        if (!HasCapture())
            CaptureMouse();
        UpdateFromMouse(event.GetX());
    }

    void OnMotion(wxMouseEvent& event)
    {
        if (m_drag)
            UpdateFromMouse(event.GetX());
    }

    void OnLeftUp(wxMouseEvent& event)
    {
        m_drag = false;
        if (HasCapture())
            ReleaseMouse();
    }

    void OnCaptureLost(wxMouseCaptureLostEvent& event)
    {
        m_drag = false;
    }

    void OnDoubleClick(wxMouseEvent& event)
    {
        m_drag = false;
        if (HasCapture())
            ReleaseMouse();

        if (m_value != m_default)
        {
            m_value = m_default;
            Refresh();
            SendChange();
        }
    }

private:
    static const int PAD = 10;

    wxString m_caption;
    int m_min;
    int m_max;
    int m_default;
    int m_value;
    bool m_drag;

    int Clamp(int value) const
    {
        if (value < m_min)
            return m_min;
        if (value > m_max)
            return m_max;
        return value;
    }

    wxString FormatValue(int value) const
    {
        if (m_min < 0 && value > 0)
            return wxString::Format("+%d", value);
        return wxString::Format("%d", value);
    }

    int ValueToX(int value) const
    {
        int x0 = PAD;
        int x1 = GetClientSize().x - PAD;
        if (m_max == m_min || x1 <= x0)
            return x0;
        return x0 + (int)((double)(value - m_min) * (x1 - x0) / (m_max - m_min));
    }

    int XToValue(int x) const
    {
        int x0 = PAD;
        int x1 = GetClientSize().x - PAD;
        if (m_max == m_min || x1 <= x0)
            return m_min;
        double t = (double)(x - x0) / (x1 - x0);
        return Clamp(m_min + wxRound(t * (m_max - m_min)));
    }

    void UpdateFromMouse(int x)
    {
        int value = XToValue(x);
        if (value != m_value)
        {
            m_value = value;
            Refresh();
            SendChange();
        }
    }

    void SendChange()
    {
        wxCommandEvent change(wxEVT_SLIDER, GetId());
        change.SetEventObject(this);
        change.SetInt(m_value);
        GetEventHandler()->ProcessEvent(change);
    }

    DECLARE_EVENT_TABLE()
};

wxBEGIN_EVENT_TABLE(FlatSlider, wxPanel)
    EVT_PAINT(FlatSlider::OnPaint)
    EVT_LEFT_DOWN(FlatSlider::OnLeftDown)
    EVT_LEFT_UP(FlatSlider::OnLeftUp)
    EVT_MOTION(FlatSlider::OnMotion)
    EVT_LEFT_DCLICK(FlatSlider::OnDoubleClick)
    EVT_MOUSE_CAPTURE_LOST(FlatSlider::OnCaptureLost)
wxEND_EVENT_TABLE()

// =====================================================================
//  القسم 8: لوحة عرض الصورة
// =====================================================================

class ImagePanel : public wxPanel
{
public:
    ImagePanel(wxWindow* parent)
        : wxPanel(parent, wxID_ANY)
    {
        m_bmpW = 0;
        m_bmpH = 0;
        m_dirty = true;
        SetBackgroundStyle(wxBG_STYLE_PAINT);
    }

    void SetImage(const wxImage& img)
    {
        m_image = img;
        m_dirty = true;
        Refresh();
    }

    void SetTexts(const wxString& title, const wxString& emptyText)
    {
        m_title = title;
        m_emptyText = emptyText;
        Refresh();
    }

    void OnPaint(wxPaintEvent& event)
    {
        wxAutoBufferedPaintDC dc(this);
        wxSize size = GetClientSize();

        dc.SetBackground(wxBrush(g_theme.bg));
        dc.Clear();

        dc.SetBrush(wxBrush(g_theme.canvas));
        dc.SetPen(wxPen(g_theme.border, 1));
        dc.DrawRoundedRectangle(0, 0, size.x, size.y, 14);

        wxFont titleFont = GetFont();
        titleFont.SetWeight(wxFONTWEIGHT_BOLD);
        dc.SetFont(titleFont);
        dc.SetTextForeground(g_theme.subText);
        dc.DrawText(m_title, 18, 14);

        wxRect area(18, 46, size.x - 36, size.y - 64);

        if (!m_image.IsOk() || area.width < 10 || area.height < 10)
        {
            dc.SetFont(GetFont());
            dc.DrawLabel(m_emptyText, wxRect(0, 0, size.x, size.y), wxALIGN_CENTER);
            return;
        }

        double scaleX = (double)area.width / m_image.GetWidth();
        double scaleY = (double)area.height / m_image.GetHeight();
        double scale = scaleX < scaleY ? scaleX : scaleY;

        int w = (int)(m_image.GetWidth() * scale);
        int h = (int)(m_image.GetHeight() * scale);
        if (w < 1) w = 1;
        if (h < 1) h = 1;

        if (m_dirty || !m_bitmap.IsOk() || m_bmpW != w || m_bmpH != h)
        {
            wxImage scaled = m_image.Scale(w, h, wxIMAGE_QUALITY_HIGH);
            m_bitmap = wxBitmap(scaled);
            m_bmpW = w;
            m_bmpH = h;
            m_dirty = false;
        }

        int x = area.x + (area.width - w) / 2;
        int y = area.y + (area.height - h) / 2;

        dc.DrawBitmap(m_bitmap, x, y, true);

        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(wxPen(g_theme.border, 1));
        dc.DrawRectangle(x - 1, y - 1, w + 2, h + 2);
    }

    void OnSize(wxSizeEvent& event)
    {
        Refresh();
        event.Skip();
    }

private:
    wxImage m_image;
    wxBitmap m_bitmap;
    int m_bmpW;
    int m_bmpH;
    bool m_dirty;
    wxString m_title;
    wxString m_emptyText;

    DECLARE_EVENT_TABLE()
};

wxBEGIN_EVENT_TABLE(ImagePanel, wxPanel)
    EVT_PAINT(ImagePanel::OnPaint)
    EVT_SIZE(ImagePanel::OnSize)
wxEND_EVENT_TABLE()

// =====================================================================
//  القسم 9: النافذة الرئيسية
// =====================================================================

enum
{
    ID_OPEN = wxID_HIGHEST + 1,
    ID_SAVE,
    ID_APPLY,
    ID_UNDO,
    ID_REDO,
    ID_RESET,
    ID_THEME,
    ID_LANG,
    ID_CAP,
    ID_FILTER,
    ID_TIMER
};

const int ID_PARAM0 = wxID_HIGHEST + 100;
const int ID_PARAM_LAST = ID_PARAM0 + MAX_PARAMS - 1;

const int PREVIEW_MAX = 1000;

class MainFrame : public wxFrame
{
public:
    MainFrame();

    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnApply(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);
    void OnRedo(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);
    void OnTheme(wxCommandEvent& event);
    void OnLang(wxCommandEvent& event);
    void OnCapacitySpin(wxSpinEvent& event);
    void OnCapacityText(wxCommandEvent& event);
    void OnFilterChanged(wxCommandEvent& event);
    void OnParamChanged(wxCommandEvent& event);
    void OnPreviewTimer(wxTimerEvent& event);

private:
    wxPanel* m_center;
    wxScrolledWindow* m_side;
    ImagePanel* m_origPanel;
    ImagePanel* m_editPanel;

    wxStaticText* m_titleLbl;
    wxStaticText* m_secFile;
    wxStaticText* m_secFilter;
    wxStaticText* m_secOptions;
    wxStaticText* m_secHistory;
    wxStaticText* m_secSettings;
    wxStaticText* m_capLbl;
    wxStaticText* m_stepsLbl;

    wxChoice* m_filterChoice;
    wxSpinCtrl* m_capSpin;

    wxPanel* m_optPanel;
    wxBoxSizer* m_optSizer;
    FlatSlider* m_sliders[MAX_PARAMS];
    wxCheckBox* m_checks[MAX_PARAMS];
    wxChoice* m_choices[MAX_PARAMS];
    FilterParams m_params;

    FlatButton* m_openBtn;
    FlatButton* m_saveBtn;
    FlatButton* m_applyBtn;
    FlatButton* m_undoBtn;
    FlatButton* m_redoBtn;
    FlatButton* m_resetBtn;
    FlatButton* m_themeBtn;
    FlatButton* m_langBtn;

    wxImage m_original;
    wxImage m_current;
    std::vector<wxImage> m_undo;
    std::vector<wxImage> m_redo;
    int m_capacity;
    bool m_hasImage;

    wxTimer m_previewTimer;
    wxImage m_previewSrc;
    bool m_previewSrcValid;
    bool m_previewing;

    wxStaticText* MakeSectionLabel();
    void ApplyTheme();
    void ApplyLanguage();
    void UpdateViews();
    void UpdateButtons();
    void TrimHistory();
    void PushUndo(const wxImage& img);
    void UpdateCapacity();

    int CurrentFilter();
    void ResetParams(int index);
    void ReadParams();
    void RebuildOptions(bool keepValues);
    void EnsurePreviewSource();
    void DoPreview();
    void ShowCurrent();

    DECLARE_EVENT_TABLE()
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(ID_OPEN,  MainFrame::OnOpen)
    EVT_BUTTON(ID_SAVE,  MainFrame::OnSave)
    EVT_BUTTON(ID_APPLY, MainFrame::OnApply)
    EVT_BUTTON(ID_UNDO,  MainFrame::OnUndo)
    EVT_BUTTON(ID_REDO,  MainFrame::OnRedo)
    EVT_BUTTON(ID_RESET, MainFrame::OnReset)
    EVT_BUTTON(ID_THEME, MainFrame::OnTheme)
    EVT_BUTTON(ID_LANG,  MainFrame::OnLang)
    EVT_MENU(ID_OPEN,    MainFrame::OnOpen)
    EVT_MENU(ID_SAVE,    MainFrame::OnSave)
    EVT_MENU(ID_UNDO,    MainFrame::OnUndo)
    EVT_MENU(ID_REDO,    MainFrame::OnRedo)
    EVT_SPINCTRL(ID_CAP, MainFrame::OnCapacitySpin)
    EVT_TEXT(ID_CAP,     MainFrame::OnCapacityText)
    EVT_CHOICE(ID_FILTER, MainFrame::OnFilterChanged)
    EVT_COMMAND_RANGE(ID_PARAM0, ID_PARAM_LAST, wxEVT_SLIDER,   MainFrame::OnParamChanged)
    EVT_COMMAND_RANGE(ID_PARAM0, ID_PARAM_LAST, wxEVT_CHECKBOX, MainFrame::OnParamChanged)
    EVT_COMMAND_RANGE(ID_PARAM0, ID_PARAM_LAST, wxEVT_CHOICE,   MainFrame::OnParamChanged)
    EVT_TIMER(ID_TIMER,  MainFrame::OnPreviewTimer)
wxEND_EVENT_TABLE()

wxStaticText* MainFrame::MakeSectionLabel()
{
    wxStaticText* label = new wxStaticText(m_side, wxID_ANY, "");
    wxFont font = label->GetFont();
    font.SetWeight(wxFONTWEIGHT_BOLD);
    label->SetFont(font);
    return label;
}

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "Photo Editor", wxDefaultPosition, wxSize(1240, 800)),
      m_previewTimer(this, ID_TIMER)
{
    SetMinSize(wxSize(920, 600));
    m_capacity = 20;
    m_hasImage = false;
    m_previewSrcValid = false;
    m_previewing = false;
    for (int i = 0; i < MAX_PARAMS; i++)
    {
        m_sliders[i] = NULL;
        m_checks[i] = NULL;
        m_choices[i] = NULL;
    }
    ResetParams(0);
    LoadTheme();

    wxBoxSizer* root = new wxBoxSizer(wxHORIZONTAL);

    m_center = new wxPanel(this, wxID_ANY);
    wxBoxSizer* centerSizer = new wxBoxSizer(wxHORIZONTAL);

    m_origPanel = new ImagePanel(m_center);
    m_editPanel = new ImagePanel(m_center);

    centerSizer->Add(m_origPanel, 1, wxEXPAND | wxALL, 14);
    centerSizer->Add(m_editPanel, 1, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 14);
    m_center->SetSizer(centerSizer);

    m_side = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(300, -1), wxVSCROLL);
    m_side->SetMinSize(wxSize(300, -1));
    m_side->SetScrollRate(0, 12);
    wxBoxSizer* side = new wxBoxSizer(wxVERTICAL);

    m_titleLbl = new wxStaticText(m_side, wxID_ANY, "");
    wxFont titleFont = m_titleLbl->GetFont();
    titleFont.SetPointSize(titleFont.GetPointSize() + 6);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    m_titleLbl->SetFont(titleFont);
    side->Add(m_titleLbl, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 18);

    m_secFile = MakeSectionLabel();
    side->Add(m_secFile, 0, wxLEFT | wxRIGHT, 18);

    m_openBtn = new FlatButton(m_side, ID_OPEN, "", true);
    m_saveBtn = new FlatButton(m_side, ID_SAVE, "", false);
    wxBoxSizer* fileRow = new wxBoxSizer(wxHORIZONTAL);
    fileRow->Add(m_openBtn, 1, wxEXPAND | wxRIGHT, 8);
    fileRow->Add(m_saveBtn, 1, wxEXPAND);
    side->AddSpacer(10);
    side->Add(fileRow, 0, wxEXPAND | wxLEFT | wxRIGHT, 18);
    side->AddSpacer(14);

    m_secFilter = MakeSectionLabel();
    side->Add(m_secFilter, 0, wxLEFT | wxRIGHT, 18);

    m_filterChoice = new wxChoice(m_side, ID_FILTER);
    m_filterChoice->SetMinSize(wxSize(-1, 34));
    side->Add(m_filterChoice, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 18);
    side->AddSpacer(10);

    m_secOptions = MakeSectionLabel();
    side->Add(m_secOptions, 0, wxLEFT | wxRIGHT | wxTOP, 18);

    m_optPanel = new wxPanel(m_side, wxID_ANY);
    m_optSizer = new wxBoxSizer(wxVERTICAL);
    m_optPanel->SetSizer(m_optSizer);
    side->Add(m_optPanel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 18);

    m_applyBtn = new FlatButton(m_side, ID_APPLY, "", true);
    side->Add(m_applyBtn, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 18);
    side->AddSpacer(14);

    m_secHistory = MakeSectionLabel();
    side->Add(m_secHistory, 0, wxLEFT | wxRIGHT, 18);

    m_undoBtn = new FlatButton(m_side, ID_UNDO, "", false);
    m_redoBtn = new FlatButton(m_side, ID_REDO, "", false);
    wxBoxSizer* histRow = new wxBoxSizer(wxHORIZONTAL);
    histRow->Add(m_undoBtn, 1, wxEXPAND | wxRIGHT, 8);
    histRow->Add(m_redoBtn, 1, wxEXPAND);
    side->Add(histRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 18);
    side->AddSpacer(8);

    m_resetBtn = new FlatButton(m_side, ID_RESET, "", false);
    side->Add(m_resetBtn, 0, wxEXPAND | wxLEFT | wxRIGHT, 18);
    side->AddSpacer(12);

    m_capLbl = new wxStaticText(m_side, wxID_ANY, "");
    m_capSpin = new wxSpinCtrl(m_side, ID_CAP, "20", wxDefaultPosition, wxSize(110, -1),
                               wxSP_ARROW_KEYS, 1, 100, 20);
    wxBoxSizer* capRow = new wxBoxSizer(wxHORIZONTAL);
    capRow->Add(m_capLbl, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 8);
    capRow->Add(m_capSpin, 0, wxALIGN_CENTER_VERTICAL);
    side->Add(capRow, 0, wxEXPAND | wxLEFT | wxRIGHT, 18);

    m_stepsLbl = new wxStaticText(m_side, wxID_ANY, "");
    side->AddSpacer(8);
    side->Add(m_stepsLbl, 0, wxLEFT | wxRIGHT, 18);

    side->AddStretchSpacer(1);

    m_secSettings = MakeSectionLabel();
    side->AddSpacer(10);
    side->Add(m_secSettings, 0, wxLEFT | wxRIGHT, 18);

    m_themeBtn = new FlatButton(m_side, ID_THEME, "", false);
    m_langBtn = new FlatButton(m_side, ID_LANG, "", false);
    wxBoxSizer* setRow = new wxBoxSizer(wxHORIZONTAL);
    setRow->Add(m_themeBtn, 1, wxEXPAND | wxRIGHT, 8);
    setRow->Add(m_langBtn, 1, wxEXPAND);
    side->AddSpacer(12);
    side->Add(setRow, 0, wxEXPAND | wxLEFT | wxRIGHT, 18);
    side->AddSpacer(16);

    m_side->SetSizer(side);

    root->Add(m_center, 1, wxEXPAND);
    root->Add(m_side, 0, wxEXPAND);
    SetSizer(root);

    wxAcceleratorEntry entries[4];
    entries[0].Set(wxACCEL_CTRL, (int)'O', ID_OPEN);
    entries[1].Set(wxACCEL_CTRL, (int)'S', ID_SAVE);
    entries[2].Set(wxACCEL_CTRL, (int)'Z', ID_UNDO);
    entries[3].Set(wxACCEL_CTRL, (int)'Y', ID_REDO);
    wxAcceleratorTable accel(4, entries);
    SetAcceleratorTable(accel);

    ApplyTheme();
    ApplyLanguage();
    UpdateViews();
    UpdateButtons();
    Centre();
}

void MainFrame::ApplyTheme()
{
    LoadTheme();

#if wxCHECK_VERSION(3, 3, 0)
    wxTheApp->SetAppearance(g_dark ? wxApp::Appearance::Dark : wxApp::Appearance::Light);
#endif

    SetBackgroundColour(g_theme.bg);
    m_center->SetBackgroundColour(g_theme.bg);
    m_side->SetBackgroundColour(g_theme.panel);
    m_optPanel->SetBackgroundColour(g_theme.panel);

    m_titleLbl->SetBackgroundColour(g_theme.panel);
    m_titleLbl->SetForegroundColour(g_theme.accent);

    m_secFile->SetBackgroundColour(g_theme.panel);
    m_secFile->SetForegroundColour(g_theme.subText);
    m_secFilter->SetBackgroundColour(g_theme.panel);
    m_secFilter->SetForegroundColour(g_theme.subText);
    m_secOptions->SetBackgroundColour(g_theme.panel);
    m_secOptions->SetForegroundColour(g_theme.subText);
    m_secHistory->SetBackgroundColour(g_theme.panel);
    m_secHistory->SetForegroundColour(g_theme.subText);
    m_secSettings->SetBackgroundColour(g_theme.panel);
    m_secSettings->SetForegroundColour(g_theme.subText);

    m_capLbl->SetBackgroundColour(g_theme.panel);
    m_capLbl->SetForegroundColour(g_theme.text);
    m_stepsLbl->SetBackgroundColour(g_theme.panel);
    m_stepsLbl->SetForegroundColour(g_theme.subText);

    m_filterChoice->SetBackgroundColour(g_theme.field);
    m_filterChoice->SetForegroundColour(g_theme.text);
    m_capSpin->SetBackgroundColour(g_theme.field);
    m_capSpin->SetForegroundColour(g_theme.text);

    m_themeBtn->SetCaption(g_dark ? Tr(S_LIGHT_MODE) : Tr(S_DARK_MODE));

    ReadParams();
    RebuildOptions(true);

    Refresh();
    m_side->Refresh();
    m_center->Refresh();
    m_origPanel->Refresh();
    m_editPanel->Refresh();
}

void MainFrame::ApplyLanguage()
{
    ReadParams();

    SetTitle(Tr(S_TITLE));
    m_titleLbl->SetLabel(Tr(S_TITLE));

    m_secFile->SetLabel(Tr(S_FILE));
    m_secFilter->SetLabel(Tr(S_FILTERS));
    m_secOptions->SetLabel(Tr(S_OPTIONS));
    m_secHistory->SetLabel(Tr(S_HISTORY));
    m_secSettings->SetLabel(Tr(S_SETTINGS));
    m_capLbl->SetLabel(Tr(S_CAPACITY));

    m_openBtn->SetCaption(Tr(S_OPEN));
    m_saveBtn->SetCaption(Tr(S_SAVE));
    m_applyBtn->SetCaption(Tr(S_APPLY));
    m_undoBtn->SetCaption(Tr(S_UNDO));
    m_redoBtn->SetCaption(Tr(S_REDO));
    m_resetBtn->SetCaption(Tr(S_RESET));
    m_themeBtn->SetCaption(g_dark ? Tr(S_LIGHT_MODE) : Tr(S_DARK_MODE));

    if (g_arabic)
        m_langBtn->SetCaption("English");
    else
        m_langBtn->SetCaption(wxString::FromUTF8("العربية"));

    int selected = m_filterChoice->GetSelection();
    m_filterChoice->Clear();
    for (int i = 0; i < FILTER_COUNT; i++)
    {
        if (g_arabic)
            m_filterChoice->Append(wxString::FromUTF8(g_filters[i].nameAr));
        else
            m_filterChoice->Append(wxString::FromUTF8(g_filters[i].nameEn));
    }
    if (selected == wxNOT_FOUND)
        selected = 0;
    m_filterChoice->SetSelection(selected);

    RebuildOptions(true);

    m_origPanel->SetTexts(Tr(S_ORIGINAL), Tr(S_NO_IMAGE));
    m_editPanel->SetTexts(m_previewing ? Tr(S_PREVIEW) : Tr(S_EDITED), Tr(S_NO_IMAGE));

    UpdateButtons();
    m_side->Layout();
    Layout();
    Refresh();
}

int MainFrame::CurrentFilter()
{
    int index = m_filterChoice->GetSelection();
    if (index == wxNOT_FOUND || index < 0 || index >= FILTER_COUNT)
        return 0;
    return index;
}

void MainFrame::ResetParams(int index)
{
    for (int i = 0; i < MAX_PARAMS; i++)
        m_params.v[i] = 0;

    for (int i = 0; i < g_filters[index].paramCount; i++)
        m_params.v[i] = g_filters[index].params[i].defV;
}

void MainFrame::ReadParams()
{
    int index = CurrentFilter();
    for (int i = 0; i < g_filters[index].paramCount; i++)
    {
        if (m_sliders[i])
        {
            m_params.v[i] = m_sliders[i]->GetValue();
        }
        else if (m_checks[i])
        {
            m_params.v[i] = m_checks[i]->GetValue() ? 1 : 0;
        }
        else if (m_choices[i])
        {
            int sel = m_choices[i]->GetSelection();
            m_params.v[i] = (sel == wxNOT_FOUND) ? 0 : sel;
        }
    }
}

void MainFrame::RebuildOptions(bool keepValues)
{
    int index = CurrentFilter();
    const FilterDef& filter = g_filters[index];

    if (!keepValues)
        ResetParams(index);

    m_optSizer->Clear(true);
    for (int i = 0; i < MAX_PARAMS; i++)
    {
        m_sliders[i] = NULL;
        m_checks[i] = NULL;
        m_choices[i] = NULL;
    }

    for (int i = 0; i < filter.paramCount; i++)
    {
        const ParamDef& def = filter.params[i];
        int id = ID_PARAM0 + i;

        wxString name;
        if (g_arabic)
            name = wxString::FromUTF8(def.nameAr);
        else
            name = wxString::FromUTF8(def.nameEn);

        if (def.type == PARAM_SLIDER)
        {
            FlatSlider* slider = new FlatSlider(m_optPanel, id, name,
                                                def.minV, def.maxV, def.defV);
            slider->SetValue(m_params.v[i]);
            slider->SetToolTip(Tr(S_DBL_RESET));
            m_optSizer->Add(slider, 0, wxEXPAND | wxBOTTOM, 8);
            m_sliders[i] = slider;
        }
        else if (def.type == PARAM_CHECK)
        {
            wxCheckBox* check = new wxCheckBox(m_optPanel, id, name);
            check->SetBackgroundColour(g_theme.panel);
            check->SetForegroundColour(g_theme.text);
            check->SetValue(m_params.v[i] != 0);
            m_optSizer->Add(check, 0, wxEXPAND | wxTOP | wxBOTTOM, 6);
            m_checks[i] = check;
        }
        else
        {
            wxStaticText* label = new wxStaticText(m_optPanel, wxID_ANY, name);
            label->SetBackgroundColour(g_theme.panel);
            label->SetForegroundColour(g_theme.text);

            wxChoice* choice = new wxChoice(m_optPanel, id);
            wxString items;
            if (g_arabic)
                items = wxString::FromUTF8(def.choicesAr);
            else
                items = wxString::FromUTF8(def.choicesEn);

            wxArrayString names = wxSplit(items, '|');
            for (size_t k = 0; k < names.GetCount(); k++)
                choice->Append(names[k]);

            int sel = m_params.v[i];
            if (sel < 0 || sel >= (int)names.GetCount())
                sel = 0;
            if (names.GetCount() > 0)
                choice->SetSelection(sel);

            m_optSizer->Add(label, 0, wxTOP | wxBOTTOM, 4);
            m_optSizer->Add(choice, 0, wxEXPAND | wxBOTTOM, 10);
            m_choices[i] = choice;
        }
    }

    bool hasParams = (filter.paramCount > 0);
    m_secOptions->Show(hasParams);
    m_optPanel->Show(hasParams);

    m_optPanel->Layout();
    m_side->Layout();
    m_side->FitInside();
    m_side->Refresh();
}

void MainFrame::EnsurePreviewSource()
{
    if (m_previewSrcValid)
        return;

    int w = m_current.GetWidth();
    int h = m_current.GetHeight();
    int longest = w > h ? w : h;

    if (longest > PREVIEW_MAX)
    {
        double scale = (double)PREVIEW_MAX / longest;
        int newW = (int)(w * scale);
        int newH = (int)(h * scale);
        if (newW < 1) newW = 1;
        if (newH < 1) newH = 1;
        m_previewSrc = m_current.Scale(newW, newH, wxIMAGE_QUALITY_BILINEAR);
    }
    else
    {
        m_previewSrc = m_current.Copy();
    }
    m_previewSrcValid = true;
}

void MainFrame::ShowCurrent()
{
    m_previewing = false;
    if (m_hasImage)
        m_editPanel->SetImage(m_current);
    else
        m_editPanel->SetImage(wxImage());
    m_editPanel->SetTexts(Tr(S_EDITED), Tr(S_NO_IMAGE));
}

void MainFrame::DoPreview()
{
    if (!m_hasImage)
        return;

    int index = CurrentFilter();
    if (g_filters[index].paramCount == 0)
    {
        if (m_previewing)
            ShowCurrent();
        return;
    }

    ReadParams();
    EnsurePreviewSource();

    wxImage result = g_filters[index].func(m_previewSrc, m_params);
    if (!result.IsOk())
        return;

    m_previewing = true;
    m_editPanel->SetImage(result);
    m_editPanel->SetTexts(Tr(S_PREVIEW), Tr(S_NO_IMAGE));
}

void MainFrame::OnFilterChanged(wxCommandEvent& event)
{
    m_previewTimer.Stop();
    RebuildOptions(false);
    DoPreview();
}

void MainFrame::OnParamChanged(wxCommandEvent& event)
{
    m_previewTimer.Start(60, wxTIMER_ONE_SHOT);
}

void MainFrame::OnPreviewTimer(wxTimerEvent& event)
{
    DoPreview();
}

void MainFrame::UpdateViews()
{
    m_previewSrcValid = false;

    if (m_hasImage)
        m_origPanel->SetImage(m_original);
    else
        m_origPanel->SetImage(wxImage());

    ShowCurrent();
}

void MainFrame::UpdateButtons()
{
    m_undoBtn->Enable(!m_undo.empty());
    m_redoBtn->Enable(!m_redo.empty());
    m_applyBtn->Enable(m_hasImage);
    m_resetBtn->Enable(m_hasImage);
    m_saveBtn->Enable(m_hasImage);

    wxString steps = wxString::Format("%s: %d / %d",
                                      Tr(S_STEPS),
                                      (int)m_undo.size(),
                                      m_capacity);
    m_stepsLbl->SetLabel(steps);
    m_side->Layout();
}

void MainFrame::TrimHistory()
{
    while ((int)m_undo.size() > m_capacity)
        m_undo.erase(m_undo.begin());

    while ((int)m_redo.size() > m_capacity)
        m_redo.erase(m_redo.begin());
}

void MainFrame::PushUndo(const wxImage& img)
{
    m_undo.push_back(img);
    TrimHistory();
}

void MainFrame::UpdateCapacity()
{
    int value = m_capSpin->GetValue();
    if (value < 1)
        value = 1;
    m_capacity = value;
    TrimHistory();
    UpdateButtons();
}

void MainFrame::OnCapacitySpin(wxSpinEvent& event)
{
    UpdateCapacity();
}

void MainFrame::OnCapacityText(wxCommandEvent& event)
{
    UpdateCapacity();
}

void MainFrame::OnUndo(wxCommandEvent& event)
{
    if (m_undo.empty())
        return;

    m_previewTimer.Stop();
    m_redo.push_back(m_current);
    m_current = m_undo.back();
    m_undo.pop_back();
    TrimHistory();

    UpdateViews();
    UpdateButtons();
}

void MainFrame::OnRedo(wxCommandEvent& event)
{
    if (m_redo.empty())
        return;

    m_previewTimer.Stop();
    m_undo.push_back(m_current);
    m_current = m_redo.back();
    m_redo.pop_back();
    TrimHistory();

    UpdateViews();
    UpdateButtons();
}

void MainFrame::OnOpen(wxCommandEvent& event)
{
    wxFileDialog dlg(this, Tr(S_OPEN_DLG), "", "",
                     "Images|*.png;*.jpg;*.jpeg;*.bmp;*.gif;*.tif;*.tiff",
                     wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (dlg.ShowModal() != wxID_OK)
        return;

    wxImage img;
    if (!img.LoadFile(dlg.GetPath()) || !img.IsOk())
    {
        wxMessageBox(Tr(S_LOAD_FAIL), Tr(S_ERROR), wxOK | wxICON_ERROR, this);
        return;
    }

    m_previewTimer.Stop();
    m_original = img;
    m_current = img.Copy();
    m_undo.clear();
    m_redo.clear();
    m_hasImage = true;

    UpdateViews();
    UpdateButtons();
}

void MainFrame::OnSave(wxCommandEvent& event)
{
    if (!m_hasImage)
    {
        wxMessageBox(Tr(S_OPEN_FIRST), Tr(S_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }

    wxFileDialog dlg(this, Tr(S_SAVE_DLG), "", "edited",
                     "PNG (*.png)|*.png|JPEG (*.jpg)|*.jpg|BMP (*.bmp)|*.bmp",
                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (dlg.ShowModal() != wxID_OK)
        return;

    int index = dlg.GetFilterIndex();
    wxBitmapType type = wxBITMAP_TYPE_PNG;
    wxString ext = "png";
    if (index == 1)
    {
        type = wxBITMAP_TYPE_JPEG;
        ext = "jpg";
    }
    else if (index == 2)
    {
        type = wxBITMAP_TYPE_BMP;
        ext = "bmp";
    }

    wxFileName file(dlg.GetPath());
    if (!file.HasExt())
        file.SetExt(ext);

    if (!m_current.SaveFile(file.GetFullPath(), type))
        wxMessageBox(Tr(S_SAVE_FAIL), Tr(S_ERROR), wxOK | wxICON_ERROR, this);
}

void MainFrame::OnApply(wxCommandEvent& event)
{
    if (!m_hasImage)
    {
        wxMessageBox(Tr(S_OPEN_FIRST), Tr(S_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }

    int index = m_filterChoice->GetSelection();
    if (index == wxNOT_FOUND)
        return;

    m_previewTimer.Stop();
    ReadParams();

    wxImage result;
    {
        wxBusyCursor wait;
        result = g_filters[index].func(m_current, m_params);
    }
    if (!result.IsOk())
        return;

    PushUndo(m_current);
    m_redo.clear();
    m_current = result;

    RebuildOptions(false);

    UpdateViews();
    UpdateButtons();
}

void MainFrame::OnReset(wxCommandEvent& event)
{
    if (!m_hasImage)
        return;

    m_previewTimer.Stop();
    PushUndo(m_current);
    m_redo.clear();
    m_current = m_original.Copy();

    UpdateViews();
    UpdateButtons();
}

void MainFrame::OnTheme(wxCommandEvent& event)
{
    g_dark = !g_dark;
    ApplyTheme();
}

void MainFrame::OnLang(wxCommandEvent& event)
{
    g_arabic = !g_arabic;
    ApplyLanguage();
}

// =====================================================================
//  القسم 10: التطبيق
// =====================================================================

class PhotoEditorApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        wxInitAllImageHandlers();
        InitFilters();
        MainFrame* frame = new MainFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(PhotoEditorApp);

