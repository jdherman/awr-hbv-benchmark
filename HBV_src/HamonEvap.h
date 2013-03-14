#if defined(HYMOD) || defined(HBVMOD)

#ifndef __hamonevap_h
#define __hamonevap_h

#include "MOPEXData.h"

#define PI 3.1415926535

struct HamonEvap
{
    int day;
    double P;
    double dayLength;
    double eStar;
    double *PE;
};

void calculateHamonPE(MOPEXData *data, int dataIndex, int nDays, HamonEvap *evap, int startDay);

#endif

#endif //HYMOD



