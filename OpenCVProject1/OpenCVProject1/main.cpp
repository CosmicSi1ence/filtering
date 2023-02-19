#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <windows.h>

using namespace cv;
using namespace std;

void filter2d(Mat img, int k_filter) {

    Mat filter(k_filter, k_filter, CV_8UC1);    //создание матрицы фильтра
    filter = 255 / k_filter / k_filter;         //задание фильтра

    Mat img_pad(img.rows + k_filter - 1, img.cols + k_filter - 1, CV_8UC1); //создание матрицы дополненного изображения
    img_pad = 127;                                                          //заполняем серым цветом

    for (int i = 0; i < img.rows; i++){         //вставляем исходное изображение в центр серого
        uchar* img_pad_i = img_pad.ptr<uchar>(i + (k_filter - 1) / 2);
        uchar* img_i = img.ptr<uchar>(i);
        for (int j = 0; j < img.cols; j++)
            img_pad_i[j + (k_filter - 1) / 2] = img_i[j];
    }

    for (int i = 0; i < img.rows; ++i) {        //двумерная свертка дополненного изображения с матрицей фильтра
        uchar* img_i = img.ptr<uchar>(i);
        for (int j = 0; j < img.cols; ++j) {
            Mat roi(img_pad, Rect(j, i, k_filter, k_filter));
            double local_conv = 0;
            for (int i1 = 0; i1 < k_filter; ++i1) {
                uchar* roi_i = roi.ptr<uchar>(i1);
                uchar* filter_i = filter.ptr<uchar>(i1);
                for (int j1 = 0; j1 < k_filter; ++j1)
                    local_conv += (double)roi_i[j1] * (double)filter_i[j1]/255;
            }
            img_i[j] = local_conv;
        }
    }

}

int main(){
    char filename[] = "image.jpg";      //путь к файлу: ../OpenCVProject1/OpenCVProject1/image.jpg
    char fullFilename[MAX_PATH];
    GetFullPathNameA(filename, MAX_PATH, fullFilename, nullptr);

    cout << fullFilename << endl;
    Mat img = imread(fullFilename, IMREAD_GRAYSCALE); //считываем изображение из файла
                                                      //в формате градаций серого (одноканальный режим)
    namedWindow("Before filtering", WINDOW_AUTOSIZE);
    imshow("Before filtering", img);                        //выводим исходное изображение
    moveWindow("Before filtering", 0, 65);

    filter2d(img, 9);       //усредняющая фильтрация (второй параметр - длина фильтра)

    namedWindow("After filtering", WINDOW_AUTOSIZE);
    imshow("After filtering", img);                         //выводим изображение после фильтрации
    moveWindow("After filtering", img.cols + 2, 65);

    waitKey(0);
    destroyAllWindows();
    return 0;
}