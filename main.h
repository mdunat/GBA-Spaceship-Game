#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

struct object {
    int x;
    int y;
    int width;
    int height;
    int cond;
};

struct difficulty {
    int a_speed;
    int c_speed;
    int time;
};

#endif
