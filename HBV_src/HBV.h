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

#ifdef HBVMOD

#ifndef __hbv_h
#define __hbv_h

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cstdlib> 

#include "WaterObjectives.h"
//#include "Random.h"
#include "MOPEXData.h"
#include "HamonEvap.h"

#ifdef OPT_ALGORITHM
    #include "ensga2.h"
#else
    struct Individual;
#endif

using namespace std;

#define ROUNDINT(x) int(x + 0.5)
#define ROUNDDOUBLE(x) double(int(x + 0.5))

#ifndef EPS
    #define EPS 10.0E-9
#endif
#define TRUE 1
#define FALSE 0

//This macro check for equality of a double precision variable within eps
#define ISEQUAL(x,y) (x <= y+EPS && x >= y-EPS)

struct System
{
    int nacomp, nagcm, naystr, nadstr;
    int nayend, nadend;
};

struct Seltime
{
    int nyear[50], nbtag[50], netag[50];
    int nally;
};

struct Files
{
    string hbvfileName;
    string parfileName, istateName, tempfileName, precfileName, discfileName, resufileName, conffileName, etpfileName;
    string statfileName, summfileName, vaparaName, reserfileName;

    //string etfileName, detfileName; //Not used in the Fortran code

    ifstream discfile;
    ifstream tempfile;
    ifstream precfile;
    //ofstream statfile, etpfile, resufile, summfile, reserfile;

    //ifstream hbvfile;
    //ifstream parfile, istate, tempfile, precfile, discfile, resufile, conffile, etpfile;
    //ifstream statfile, summfile, etfile, detfile, vapara, reserfile;
};

struct Config
{
    //Areas
    double **ar; //[20][200];
    double *art; //[20];
    int *nSub; //[20];
    int nCatch;

    //Meanv
    double ***atemp; //[12][20][200];

    int *nFloTo; //[20];

    //Graph
    int *nodeTo; //[20];
    int nodes;

    int *nob; //[20];
    int noise;

    //Soilw
    //double **pwp; //[20][200]; //PWP
    double **lp; //Limiting soil moisture threshold for potential evap - now we are using this instead of pwp
    double **sowat; //[20][200]; //Soil water storate
    double **fcap; //[20][200]; //Soil starage capacity
    double **beta; //[20][200]; //Exponant parameter
    double **pwpini; //[20][200]; //Initial PWP
    double **fcapini; //[20][200]; //Initial maximum storage

    //Snow
    double **sdep; //[20][200]; //Snow store
    double **ldep; //Depth of liquid in snow store
    //double **ldep; //CWH fraction of snow store that retains meltwater and rain
    double **degd; //[20][200];
    double **degw; //[20][200];
    double **ttlim; //[20][200];
    double **sfcf; //Snow fall correction factor
    double **cfr; //Refreeze coefficient
    double **cwh; //Fraction of snow store that can be liquid
    double **ttint; //Temperature threshold interval around which a mix of snow/rain occurs

    //Storage
    double *stw1; //[20];
    double *stw2; //[20];
    double *hl1; //[20];
    double *ck0; //[20];
    double *ck1; //[20];
    double *ck2; //[20];
    double *cper; //[20];
    double *perc; //Percolation from upper to lower box [L/T]

    //Muskingum Routing
    double *ak; //[20];
    double *xmu; //[20];

    //Dunst
    double ***poev; //[12][20][200]
    double ***cef; //[12][20][200];
    //double poev[12][20][200];
    //double cef[12][20][200];

    //U-Hyd
    int *maxbas; //[20];

    //Timestep
    double tst;
    int nagg;

    //Loss
    //double **hloss; //[20][20];
    //double **aseal; //[20][20];

    //Subsurface
    double *ekk; //[20];

    //Some other stuff
    double *sums1; //[20];
    double *sums2; //[20];

    int ianfj, ianft, iendj, iendt;

    int nity, nita;

    double **pdep; //[20][200];
    double **tcel; //[20][200];

    //State output variables not currently needed
    //double avsobs[20][20];
    //double avsmod[20][20];
    //double vasobs[20][20];
    //double vasmod[20][20];
    //double cors[20][20];
    //double bms[20][20];
    //double erobs[20][20];
    //double ermod[20][20];
    //double Qsobs[20][20];
    //double Qsmod[20][20];
    //double nScale[20];

};

struct Flows
{
    double **Qtot; //[20][40];
    double **Qtou; //[20][40];
    double **Qind; //[20][40];
    double **Qout; //[20][40];
    double **Qtr; // [20][40];
    double **Qmodelled;
};

//struct Stats
//{
//    int ikm;
//    int indx;
//    int iamo;
//    int ismo;
//
//    double Qmo[20][600];
//    double Qmax[20][600];
//    double evp[20][600];
//    double ess[20][600];
//};

struct HBV
{
    System system;
    Seltime seltime;
    Files files;
    Config config;
    //Stats stats;
    Flows flows;
    MOPEXData data;
    HamonEvap evap;
};

//Function prototypes
void init_HBV(int argc, char** argv);
void calc_HBV(Individual *ind);
void finalize_HBV();

void readHBVSys(HBV *hbv);
void simpleConfiguration(HBV *hbv);
void readConfiguration(HBV *hbv);
void readParameters(HBV *hbv);
void readInitialConditions(HBV *hbv);
void readParameterValidation(HBV *hbv);
void readFlow(HBV *hbv, double *Qmes);

void printStat(HBV *hbv);

//Original HBV routines
void snow(double &sd, double tc, double pd, double dd, double &pe, double tt, double tdt);
void soil(double pe, double &sw, double fc, double be, double &qd);
void disch(HBV *hbv, int ic, double &Qall);
void outind(HBV *hbv, double Qall, int ic, int it);
void musking(HBV *hbv, int ic, int it);
void meteo(HBV *hbv, int &isend, int &imon);
void backflow(HBV *hbv);
void summit(HBV *hbv, int it);
void process(HBV *hbv);

//New routines
void initParameters(HBV *hbv);
void reinitForMaxBas(HBV *hbv);
void processNew(HBV *hbv, int startingIndex, int PeriodLength, int writeOutput);
void zonesNew(HBV *hbv, int ic, int dataIndex);
void mySnow(double &sd, double &ld, double sfcf, double cfr, double cwh, double tc, double pd, double dd, double &pe, double tt);
void evapNew(double &sw, double tc, double fcap, double lp, double avt, double pev, double cfa, double &etp);


#endif

#endif //HBV




