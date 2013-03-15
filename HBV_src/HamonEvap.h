/*
Copyright (C) 2011-2012 Josh Kollat, Jon Herman, Patrick Reed and others.

The HBV Benchmark Problem is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The HBV Benchmark Problem is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the HBV Benchmark Problem.  If not, see <http://www.gnu.org/licenses/>.
*/

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



