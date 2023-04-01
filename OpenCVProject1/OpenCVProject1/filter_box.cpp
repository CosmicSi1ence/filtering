#include "filter_box.h"

Fil box(int k_filter) {     //  —оздание матрицы коэффициентов усредн€ющего фильтра
    Fil filter(k_filter);
    filter = 1. / k_filter / k_filter;
    return filter;
}