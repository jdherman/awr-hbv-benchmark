#if defined(HYMOD) || defined(HBVMOD)

#include <math.h>
#include "HamonEvap.h"

void calculateHamonPE(MOPEXData *data, int dataIndex, int nDays, HamonEvap *evap, int startDay)
{
    int oldYear;
    int counter;

    //Allocate
    evap->PE        = new double [nDays];

    //Initialize the starting year
    oldYear = data->date[dataIndex][0];
    counter = startDay-1;

    //Fill out each of the arrays
    for (int i=0; i<nDays; i++)
    {
        
        //If the years hasn't changed, increment counter
        if (data->date[dataIndex+i][0] == oldYear) counter++;
        //If it has changed, reset counter - this handles leap years
        else counter = 1;

        evap->day = counter;

        evap->P = asin(0.39795*cos(0.2163108 + 2.0 * atan(0.9671396*tan(0.00860*double(evap->day-186)))));
        evap->dayLength = 24.0 - (24.0/PI)*(acos((sin(0.8333*PI/180.0)+sin(data->gageLat*PI/180.0)*sin(evap->P))/(cos(data->gageLat*PI/180.0)*cos(evap->P))));
        evap->eStar = 0.6108*exp((17.27*data->avgTemp[dataIndex+i])/(237.3+data->avgTemp[dataIndex+i]));
        evap->PE[i] = (715.5*evap->dayLength*evap->eStar/24.0)/(data->avgTemp[dataIndex+i] + 273.2);

        oldYear = data->date[dataIndex+i][0];
    }

    return;
}

#endif //HYMOD