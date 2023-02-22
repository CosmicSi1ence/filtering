#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <windows.h>
#include <cmath>

using namespace cv;
using namespace std;


class Fil {         //   ласс матрицы изображени€ - содержит указатель
public:             //  на первый пиксель, а также размеры изображени€
    double* pData;
    int cx, cy;

    Fil(int cx) {
        this->cx = cx;
        this->cy = cx;
        pData = new double[cx * cy];
    }

    Fil operator=(double value) {        //  при присванивании uchar значение
        for (int i = 0; i < cx * cy; ++i)    //  записываетс€ во все €чейки матрицы
            *(pData + i) = value;
        return *this;
    }

    class Iterator {
    public:
        double* pRow;
        double& operator[](int i) {
            return pRow[i];
        }
        Iterator(double* pRow) : pRow(pRow) {}
    };

    Iterator operator[](int i) { return pData + i * cy; }
};

class Img {         //   ласс матрицы изображени€ - содержит указатель
public:             //  на первый пиксель, а также размеры изображени€
    uchar* pData;
    int cx, cy;


    Img operator=(Mat m) {  //  при присванивании Mat копируютс€ размеры
        pData = m.data;     //  изображени€ и указатель на первый пиксель
        cx = m.rows;
        cy = m.cols;
        return *this;
    }
    Img operator=(uchar value) {        //  при присванивании uchar значение
        for (int i = 0; i < cx * cy; ++i)    //  записываетс€ во все €чейки матрицы
            *(pData + i) = value;
        return *this;
    }

    //      далее реализаци€ оператора индексации [ ][ ]:

    //  доступ к элементам матрицы: первые  [ ] - cx - по горизонтали
    //                              вторе   [ ] - cy - по вертикали
    //  пример: a[2][8] - третий столбец, дев€та€ строка

    class Iterator {
    public:
        uchar* pRow;
        uchar& operator[](int i) {
            return pRow[i];
        }
        Iterator(uchar* pRow) : pRow(pRow) {}
    };

    Iterator operator[](int i) { return pData + i * cy; }
};

Fil box(int k_filter) {     //  —оздание матрицы коэффициентов усредн€ющего фильтра
    Fil filter(k_filter);
    filter = 1. / k_filter / k_filter;
    return filter;
}

double gaus(int x, int y, double sigma) {
    double r = sqrt(pow(x, 2) + pow(y, 2));
    double g = exp(-(r * r) / (2 * sigma * sigma)) / (2 * 3.14159265358979323846 * sigma * sigma);
    return g;
}

Fil gaussian(int k_filter, double sigma) {
    Fil filter(k_filter);
    for (int i = 0; i < k_filter; ++i)
        for (int j = 0; j < k_filter; ++j)
            filter[i][j] = gaus(i - ((k_filter - 1) / 2), j - ((k_filter - 1) / 2), sigma);
    return filter;
}

void filter2d(Mat img, Fil filter) {
    
    int k_filter = filter.cx;     //  длина фильтра

    Mat img_pad(img.rows + k_filter - 1, img.cols + k_filter - 1, CV_8UC1); //  создание матрицы дополненного изображени€
    //  ѕереводим изображени€ в класс Img, чтобы было по ним удобно итерироватьс€ через индексацию [ ][ ]:
    Img img_padM;
    img_padM = img_pad;
    Img imgM;
    imgM = img;

    int pad = (k_filter - 1) / 2;
    for (int i = 0; i < img_padM.cx; i++) {     //  зеркалирование крев дополненного изображени€
        for (int j = 0; j < img_padM.cy; j++) {

            //  углы изображени€
            if (i < pad && j < pad)
                img_padM[i][j] = imgM[-i+(pad-1)][-j+(pad-1)];
            if (i < pad && j >= imgM.cy + pad)
                img_padM[i][j] = imgM[-i + (pad - 1)][(imgM.cy + pad)-j + (imgM.cy - 1)];
            if (i >= imgM.cx + pad && j < pad)
                img_padM[i][j] = imgM[(imgM.cx+pad)-i + (imgM.cx - 1)][-j + (pad - 1)];
            if (i >= imgM.cx + pad && j >= imgM.cy + pad)
                img_padM[i][j] = imgM[(imgM.cx + pad)-i + (imgM.cx - 1)][(imgM.cy + pad) -j + (imgM.cy - 1)];

            //  кра€ изображени€ (без углов)
            if (i < pad && j >= pad && j < imgM.cy + pad)
                img_padM[i][j] = imgM[-i+(pad-1)][j-pad];
            if (i >= pad && i < imgM.cx + pad && j < pad)
                img_padM[i][j] = imgM[i - pad][-j + (pad - 1)];
            if (i >= pad && i < imgM.cx + pad && j >= imgM.cy + pad)
                img_padM[i][j] = imgM[i - pad][(imgM.cy + pad) -j + (imgM.cy - 1)];
            if (i >= imgM.cx + pad && j >= pad && j < imgM.cy + pad)
                img_padM[i][j] = imgM[(imgM.cx + pad) -i + (imgM.cx - 1)][j - pad];
            
            //  основна€ область - без отражени€
            if (i >= pad && i < imgM.cx + pad && j >= pad && j < imgM.cy + pad)
                img_padM[i][j] = imgM[i - pad][j - pad];
        }
    }
    bool togglePad = true;     // true/false , чтобы вкл./выкл. отображение дополненного изображени€
    if (togglePad) {
        namedWindow("|TEST| Padded image", WINDOW_AUTOSIZE);
        imshow("|TEST| Padded image", img_pad);                    //   выводим дополненное изображение
        moveWindow("|TEST| Padded image", 2 * (img.cols + 1), 65);
    }


    double kk = 0;
    for (int i = 0; i < k_filter; ++i) {
        for (int j = 0; j < k_filter; ++j) {
            kk += filter[i][j];
            //cout << filter[i][j] << "\t\t";   //вывод коэффициентов фильтра
        }
        //cout << endl;
    }
    cout << endl << "Brightness loss:\t" << (1 - kk) * 100 << "%" << endl << endl;

    for (int i = 0; i < imgM.cx; ++i) {        //  двумерна€ свертка дополненного изображени€ с матрицей фильтра
        for (int j = 0; j < imgM.cy; ++j) {
            Mat roi(img_pad, Rect(j, i, k_filter, k_filter));
            double local_conv = 0;
            for (int i1 = 0; i1 < k_filter; ++i1) {
                uchar* roi_i = roi.ptr<uchar>(i1);
                for (int j1 = 0; j1 < k_filter; ++j1)
                    local_conv += (double)roi_i[j1] * filter[i1][j1];
            }
            imgM[i][j] = local_conv;
        }
    }

}


int main(){
    ShowWindow(GetConsoleWindow(), SW_HIDE);    //     пр€чем назойливую консоль (переключение SH_SHOW / SH_HIDE)

    char filename[] = "image.jpg";      //  путь к файлу: ../OpenCVProject1/OpenCVProject1/image.jpg
    char fullFilename[MAX_PATH];
    GetFullPathNameA(filename, MAX_PATH, fullFilename, nullptr);

    cout << "Image file path:\t" << fullFilename << endl << endl;
    Mat img = imread(fullFilename, IMREAD_GRAYSCALE);   //  считываем изображение из файла
                                                        //  в формате градаций серого (одноканальный режим)

    namedWindow("Before filtering", WINDOW_AUTOSIZE);
    imshow("Before filtering", img);                        //  выводим исходное изображение
    moveWindow("Before filtering", 0, 65);

    //  ¬ыбор фильтра (нужное раскомментить, ненужное закомментить):
    //Fil filter = box(9);
    Fil filter = gaussian(13, 2.5);

    filter2d(img, filter);       //  фильтраци€

    namedWindow("After filtering", WINDOW_AUTOSIZE);
    imshow("After filtering", img);                         //  выводим изображение после фильтрации
    moveWindow("After filtering", img.cols + 1, 65);

    waitKey(0);
    destroyAllWindows();
    return 0;

}