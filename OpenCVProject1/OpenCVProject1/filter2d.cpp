#include "filter2d.h"

//#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "class_img.h"
#include <windows.h>

using namespace cv;

void filter2d(Mat img, Fil filter) {

    int k_filter = filter.cx;     //  длина фильтра

    Mat img_pad(img.rows + k_filter - 1, img.cols + k_filter - 1, CV_8UC1); //  создание матрицы дополненного изображения

    //  Переводим изображения в класс Img, чтобы было по ним удобно итерироваться через индексацию [ ][ ]:
    Img img_padM;
    img_padM = img_pad;
    Img imgM;
    imgM = img;

    int pad = (k_filter - 1) / 2;
    for (int i = 0; i < img_padM.cx; i++) {     //  зеркалирование крев дополненного изображения + заполнение центра исходным
        for (int j = 0; j < img_padM.cy; j++) {

            //  углы изображения
            if (i < pad && j < pad)
                img_padM[i][j] = imgM[-i + (pad - 1)][-j + (pad - 1)];
            if (i < pad && j >= imgM.cy + pad)
                img_padM[i][j] = imgM[-i + (pad - 1)][(imgM.cy + pad) - j + (imgM.cy - 1)];
            if (i >= imgM.cx + pad && j < pad)
                img_padM[i][j] = imgM[(imgM.cx + pad) - i + (imgM.cx - 1)][-j + (pad - 1)];
            if (i >= imgM.cx + pad && j >= imgM.cy + pad)
                img_padM[i][j] = imgM[(imgM.cx + pad) - i + (imgM.cx - 1)][(imgM.cy + pad) - j + (imgM.cy - 1)];

            //  края изображения (без углов)
            if (i < pad && j >= pad && j < imgM.cy + pad)
                img_padM[i][j] = imgM[-i + (pad - 1)][j - pad];
            if (i >= pad && i < imgM.cx + pad && j < pad)
                img_padM[i][j] = imgM[i - pad][-j + (pad - 1)];
            if (i >= pad && i < imgM.cx + pad && j >= imgM.cy + pad)
                img_padM[i][j] = imgM[i - pad][(imgM.cy + pad) - j + (imgM.cy - 1)];
            if (i >= imgM.cx + pad && j >= pad && j < imgM.cy + pad)
                img_padM[i][j] = imgM[(imgM.cx + pad) - i + (imgM.cx - 1)][j - pad];

            //  основная область - без отражения
            if (i >= pad && i < imgM.cx + pad && j >= pad && j < imgM.cy + pad)
                img_padM[i][j] = imgM[i - pad][j - pad];
        }
    }
    bool togglePad = true;     // true/false , чтобы вкл./выкл. отображение дополненного изображения
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
    //cout << endl << "Brightness loss:\t" << (1 - kk) * 100 << "%" << endl << endl;

    for (int i = 0; i < imgM.cx; ++i) {        //  двумерная свертка дополненного изображения с матрицей фильтра
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