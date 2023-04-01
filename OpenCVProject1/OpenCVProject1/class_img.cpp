#include "class_img.h"

Img Img::operator=(cv::Mat m) {  //  при присванивании Mat копируются размеры
    pData = m.data;     //  изображения и указатель на первый пиксель
    cx = m.rows;
    cy = m.cols;
    return *this;
}

Img Img::operator=(uchar value) {        //  при присванивании uchar значение
    for (int i = 0; i < cx * cy; ++i)    //  записывается во все ячейки матрицы
        *(pData + i) = value;
    return *this;
}

uchar& Img::Iterator::operator[](int i) {
    return pRow[i];
}

Img::Iterator Img::operator[](int i) { return pData + i * cy; }