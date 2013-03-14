/****************************************************************************
 HBV-IWS
     C VERSION OF HBV
             Distributed version
             Subcatchments and Zones
             Muskingum flood routing
             Possibility for automatic parameter optimization

             Fortran version written by A.Bardossy
             V 0.1           Stuttgart, 07.11.1998
             V 0.2           Stuttgart  09.01.1999
             Translated to C/C++ by Joshua B. Kollat
             V 0.3           Penn State University 12.2010
*****************************************************************************/


#include <stdio.h>
#include <cstdlib> 
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <math.h>
#include <stdarg.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>
#include <time.h>
#include <iostream>
#include <fstream>

#include "HBV.h"
#include "HamonEvap.h"
//#include "Random.h"
#include "WaterObjectives.h"

int writeOutput = 0;
int numSamples = 1; //Number of samples
int nParams = 14;       //Number of parameters
int nObj = 4;          //Number of performance objectives
int nTagAlongs = 0;    //Number of tag along variables
//string outputFile = "output.out";     //Name of output file

//Create an abbreviated Individual structure 
struct Individual {

     double *xreal;
     double *obj;
     double *tag_along;

};

Individual ind;
Individual* indPtr = &ind;

//Seed the random number generators
//NRRand uRand(125698); //uniform random number generator
//NRNormal nRand(0.0, 1.0, 652375); //normal random number generator


HBV hbv;
HBV *hbvPtr = &hbv;

Objectives objs;
Objectives *objsPtr = &objs;

int oldSnow = 0;

//These were settings for 7 year calibration period
/*
int dateStart[3] = {1961, 10, 1};
int dateEnd[3]   = {1969, 9, 30};
int dayStartIndex = 274; //10-1 is day 274 of the year
//Note: For the calibration period specification, you should include the warmup period here
//The objective values get calculated on the time series values following the first year of simulation
int nDays = 2922; //8 years total - 1 year of warmup, 7 years of calibration, and 2 leap years.
int startingIndex = 5023-1; //The starting index of the data array corresponding with the start date
*/

//These are settings for a 10 year calibration period
int dateStart[3] = {1961, 10, 1};
int dateEnd[3]   = {1972, 9, 30};
int dayStartIndex = 274; //10-1 is day 274 of the year
//Note: For the calibration period specification, you should include the warmup period here
//The objective values get calculated on the time series values following the first year of simulation
int nDays = 4017; //11 years total - 1 year of warmup, 10 years of calibration, and 2 leap years.
int startingIndex = 5023-1; //The starting index of the data array corresponding with the start date

int PeriodLength;
int *Period;

int lolo = 0; //This is some sort of flag that was never defined in the Fortran code

void init_HBV(int argc, char **argv)
{
  //FILE *fp = fopen("initting.out", "w");
	//fprintf(fp, "STARTING INIT...\n"); //output objectives to text file
	//fclose(fp);
	
    //Set up simulation period
    PeriodLength = nDays;
    //PeriodLength = DayLast - DayFirst + 1;
    Period = new int[PeriodLength];
    for (int d=0; d<PeriodLength; d++)
    {
        Period[d] = startingIndex+d;
    }

    //If not, they are sized according to numSamples
    allocateObjectives(objsPtr, numSamples, PeriodLength);

    simpleConfiguration(hbvPtr);
    readMOPEXData(&hbv.data, "WIL.in");
    initParameters(hbvPtr);

    //Calculate the Hamon Potential Evaporation for the time series
    calculateHamonPE(&hbv.data, startingIndex, nDays, &hbv.evap, dayStartIndex);

    //Allocate the array used to store the modelled Q
    hbvPtr->flows.Qmodelled = new double* [hbvPtr->config.nodes];
    for (int i=0; i<hbvPtr->config.nodes; i++)
    {
        hbvPtr->flows.Qmodelled[i] = new double [PeriodLength];
    }

    //Be sure and move the DA
    hbvPtr->config.ar[0][0] = hbvPtr->data.DA;
    hbvPtr->config.art[0] = hbvPtr->data.DA;

    if (writeOutput)
    {
        hbvPtr->files.reserfileName = "RESERVOIR_lumped.txt";
        hbvPtr->files.resufileName  = "SIMOBSDIS_lumped.txt";
    }

    return;
}

void calc_HBV(Individual *ind)
{
    //Create pointers to the real variables and objective values of the individual
    double *xreal  = ind->xreal;
    double *obj    = ind->obj;
    double *tag_along = ind->tag_along;

    //Reinitialize everything to zero
    hbvPtr->config.stw1  [0] = 0.0;
    hbvPtr->config.stw2  [0] = 0.0;
    hbvPtr->config.sums1 [0] = 0.0;
    hbvPtr->config.sums2 [0] = 0.0;
    hbvPtr->config.sowat[0][0]   = 0.0;
    hbvPtr->config.pwpini[0][0]  = 0.0;
    hbvPtr->config.fcapini[0][0] = 0.0;
    hbvPtr->config.sdep[0][0]    = 0.0;
    hbvPtr->config.pdep[0][0]    = 0.0;
    hbvPtr->config.tcel[0][0]    = 0.0;

    for (int i=0; i<12; i++) 
    {
        hbvPtr->config.cef[i][0][0]   = 0.0;
        hbvPtr->config.atemp[i][0][0] = 0.0;
        hbvPtr->config.poev[i][0][0]  = 0.0;
    }

    //Put the variables associated with the individual into the HyMod parameter structure
    hbvPtr->config.hl1[0]     = xreal[0]; //[mm]
    hbvPtr->config.ck0[0]     = 1.0 / xreal[1] * hbvPtr->config.tst / (3600.0 * 24.0); //1/[d]*[d] = [-]
    hbvPtr->config.ck1[0]     = 1.0 / xreal[2] * hbvPtr->config.tst / (3600.0 * 24.0); //1/[d]*[d] = [-]
    hbvPtr->config.ck2[0]     = 1.0 / xreal[3] * hbvPtr->config.tst / (3600.0 * 24.0); //1/[d]*[d] = [-]
    //hbvPtr->config.cper[0]    = 1.0 / xreal[4] * hbvPtr->config.tst / (3600.0 * 24.0); //1/[d]*[d] = [-]
    hbvPtr->config.perc[0]    = xreal[4]; //[mm/d] Now percolation is in units of mm.d instead of being specified as a percolation coefficient
    //hbvPtr->config.pwp[0][0]  = xreal[5]; //[mm]
    hbvPtr->config.lp[0][0]   = xreal[5]; //[-]
    hbvPtr->config.fcap[0][0] = xreal[6]; //[mm]
    hbvPtr->config.beta[0][0] = xreal[7]; //[-]
    hbvPtr->config.maxbas[0]  = ROUNDINT((xreal[8] * hbvPtr->config.nagg) / 24); //[d] This was originally being truncated to an int...
    hbvPtr->config.ttlim[0][0]= xreal[9]; //[degC]
    hbvPtr->config.degd[0][0] = xreal[10] * hbvPtr->config.tst / (3600.0 * 24.0); //[mm/(degC-d)]
    
    //The remaining parameters depend on whether we are running the old snow model or the new one
    if (oldSnow) hbvPtr->config.degw[0][0] = xreal[11]; //[??]
    else
    {
        //hbvPtr->config.sfcf[0][0]  = xreal[11];
        hbvPtr->config.sfcf[0][0] = 1.0; //Assuming no snowfall correction
        hbvPtr->config.cfr[0][0]   = xreal[11];
        hbvPtr->config.cwh[0][0]   = xreal[12];
        hbvPtr->config.ttint[0][0] = xreal[13];
    }

    //After we have read in maxbas, make sure we reinitialize the arrays based on maxbas
    reinitForMaxBas(hbvPtr);

    //Process data
    processNew(hbvPtr, startingIndex, PeriodLength, writeOutput);

    calculateObjectives(objsPtr, hbvPtr->flows.Qmodelled[0], hbvPtr->data.flow, hbvPtr->data.precip, startingIndex, PeriodLength, 0);

    //Fill out the individuals objective values
    obj[0] = objs.nse[0];
    obj[1] = objs.trmse[0];
    obj[2] = objs.roce[0];
    obj[3] = objs.sfdce[0];

    return;
}

void finalize_HBV()
{

    return;
}

//Main function for when we are just running HBV by itself using a different sampling technique
int main(int argc, char **argv)
{

	//JDH: THIS VERSION OF HBV IS FOR DAVE'S JAVA MOEA FRAMEWORK
	//FILE *fp = fopen("stuff.out", "w");
	//fprintf(fp, "STARTING MAIN\n"); //output objectives to text file

	int response; 

    //Allocate the "Individual" which holds the performance objs, paramets, and tag alongs;
    indPtr->obj       = new double [nObj];
    indPtr->xreal     = new double [nParams];
	indPtr->tag_along = new double [nTagAlongs];

    //Initialize HBV
    init_HBV(argc, argv);

	//initialization
	//fprintf(fp, "Before the while loop...\n"); //output objectives to text file

	//char mystring [1000];

	//ofstream teststream;
	//teststream.open("tester.txt");
	           
	//int i=0;
	while (1) //loop to wait for STDIN from java side
	{
		//if(i % 100 == 0)
		//{
			//fprintf(fp, "Evaluation: %d\n", i); //output objectives to text file
			//fflush(fp);
		//}
		//fgets (mystring, 1000, stdin);
		//fprintf(fp, "%s", mystring);
		//fflush(fp);

		response = fscanf(stdin,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
			&indPtr->xreal[0], &indPtr->xreal[1], &indPtr->xreal[2], &indPtr->xreal[3], 
			&indPtr->xreal[4], &indPtr->xreal[5], &indPtr->xreal[6], &indPtr->xreal[7], 
			&indPtr->xreal[8], &indPtr->xreal[9], &indPtr->xreal[10], &indPtr->xreal[11], 
			&indPtr->xreal[12], &indPtr->xreal[13]);
		
		//for (int j = 0; j < nParams; j++) {teststream << indPtr->xreal[j] << "  ";}
		//teststream << endl;

		if ((response == EOF) && (ferror(stdin) == 0)) 
		{
			//End of stream reached, connection closed successfully
			exit(0);
		} 
		else if (response == EOF) 
		{
			//An I/O error occurred
			fprintf(stderr, "An I/O error occurred: %s\n", strerror(errno));
			exit(-1);
		} 
		else if (response != 14) 
		{
			//Invalid number of decision variables in input
			fprintf(stderr, "Expected 14 inputs on a line!\n");
			exit(-1);
		} 
		else 
		{
			//Correct number of inputs; evaluate and output objectives back to java 

			//RUN THE MODEL
			calc_HBV(indPtr);
			//for (int j = 0; j < nObj; j++) {teststream << indPtr->obj[j] << " ";}
			//teststream << endl;
			
			fprintf(stdout, "%e %e %e %e\n", indPtr->obj[0], indPtr->obj[1], indPtr->obj[2], indPtr->obj[3]); //output back to java via STDOUT
			fflush(stdout);
			
		}	
		//i++;
	}

    return 0;
}



