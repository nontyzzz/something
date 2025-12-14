#include "data.h"

int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

int random_range(int min, int max) {
    if (min > max) return min;
    return min + (rand() % (max - min + 1));
}

int random_percent(int percent) {
    return (rand() % 100) < percent ? 1 : 0;
}
