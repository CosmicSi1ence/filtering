#include "filter_gaussian.h"
#include "func_gaus.h"

Fil gaussian(int k_filter, double sigma) {
    Fil filter(k_filter);
    for (int i = 0; i < k_filter; ++i)
        for (int j = 0; j < k_filter; ++j)
            filter[i][j] = gaus(i - ((k_filter - 1) / 2), j - ((k_filter - 1) / 2), sigma);
    return filter;
}