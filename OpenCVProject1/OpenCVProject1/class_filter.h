#pragma once

class Fil {         //  Класс матрицы изображения - содержит указатель
public:             //  на первый пиксель, а также размеры изображения
    double* pData;
    int cx, cy;

    Fil(int cx);

    Fil operator=(double value);

    class Iterator {
    public:
        double* pRow;
        double& operator[](int i);
        Iterator(double* pRow) : pRow(pRow) {}
    };

    Iterator operator[](int i);

    ~Fil();
};