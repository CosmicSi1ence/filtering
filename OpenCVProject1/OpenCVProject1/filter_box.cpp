#include "filter_box.h"

Fil box(int k_filter) {     //  �������� ������� ������������� ������������ �������
    Fil filter(k_filter);
    filter = 1. / k_filter / k_filter;
    return filter;
}