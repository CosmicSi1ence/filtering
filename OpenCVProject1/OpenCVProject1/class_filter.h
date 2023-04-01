#pragma once

class Fil {         //   ласс матрицы изображени€ - содержит указатель
public:             //  на первый пиксель, а также размеры изображени€
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