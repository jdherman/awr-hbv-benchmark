#if defined(HYMOD) || defined(HBVMOD)

#ifndef __mopexdata_h
#define __mopexdata_h

#include <string>

using namespace std;

//MOPEX data structure
struct MOPEXData
{
    string ID;
    double gageLat;
    double gageLong;
    double DA;

    //Starting and ending dates of data to read
    int *dateStart;
    int *dateEnd;

    int nDays;          //Number of days of data
    int **date;         //Date of data [year, month, day]
    double *precip;     //Mean areal precipitation (mm)
    double *evap;       //Climatic potential evaporation (mm)
    double *flow;       //Streamflow discharge (mm)
    double *maxTemp;    //Maximum air temperature (Celsius) (should be daily)
    double *minTemp;    //Minimum air temperature (Celsius) (should be daily)
    double *avgTemp;    //Average air temperature (Celsius) (should be daily)
    //double *peAdjust;  //PE adjustment factors for each month
};

void readMOPEXData(MOPEXData *data, string filename);

#endif

#endif //HYMOD


