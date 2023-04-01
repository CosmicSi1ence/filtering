#include "func_gaus.h"

double gaus(int x, int y, double sigma) {
    double r = sqrt(pow(x, 2) + pow(y, 2));
    double g = exp(-(r * r) / (2 * sigma * sigma)) / (2 * 3.14159265358979323846 * sigma * sigma);
    return g;
}