#ifndef INC_COMMON_H
#define INC_COMMON_H

#include <string>
#include "rect.h"

#include "point3.h"

#include "log.h"

#define c_pi 3.14159265

static char buf[512];

std::string IntToStr(int n);




double Abs(double sk);


template <class T>
void Swap(T &a, T &b);

template void Swap<int>(int&, int&);

#endif