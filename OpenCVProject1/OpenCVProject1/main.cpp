#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <windows.h>

#include "class_filter.h"
#include "class_img.h"
#include "func_gaus.h"
#include "filter_gaussian.h"
#include "filter2d.h"

using namespace cv;
using namespace std;

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);    //     прячем назойливую консоль (переключение SH_SHOW / SH_HIDE)

    char filename[] = "image.jpg";      //  путь к файлу: ../OpenCVProject1/OpenCVProject1/image.jpg
    char fullFilename[MAX_PATH];
    GetFullPathNameA(filename, MAX_PATH, fullFilename, nullptr);

    cout << "Image file path:\t" << fullFilename << endl << endl;
    Mat img = imread(fullFilename, IMREAD_GRAYSCALE);   //  считываем изображение из файла
    //  в формате градаций серого (одноканальный режим)

    cout << "Hello";

    namedWindow("Before filtering", WINDOW_AUTOSIZE);
    imshow("Before filtering", img);                        //  выводим исходное изображение
    moveWindow("Before filtering", 0, 65);

    //  Выбор фильтра (нужное раскомментить, ненужное закомментить):
    //Fil filter = box(9);
    Fil filter = gaussian(13, 2.5);

    filter2d(img, filter);       //  фильтрация

    namedWindow("After filtering", WINDOW_AUTOSIZE);
    imshow("After filtering", img);                         //  выводим изображение после фильтрации
    moveWindow("After filtering", img.cols + 1, 65);

    waitKey(0);
    destroyAllWindows();
    return 0;

}