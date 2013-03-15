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

#if defined(HYMOD) || defined(HBVMOD) || defined(HLRMS)

#include "WaterObjectives.h"
#include "QuickSort.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////////
// Function to calculate various modeling objectives
// Inputs:
//      Qs    = Modeled time series array
//      Qo   = Observed time series array
//      npts  = Number of points in time series
//      state = Flag for streamflow state versus others...???
// Outputs:
//      objs  = Objective values: Mean, bias, RMSE, TRMSE, HMLE
//////////////////////////////////////////////////////////////////////////

void calculateObjectives(Objectives *objs, double *Qs,double *Qo, double *precip, int dataBegin, int periodLength, int member)
{

    double sumDiffQs     = 0.0;
    double sumDiffQsSqr  = 0.0;
    double tSumDiffQsSqr = 0.0;

    double meanQoAll = 0.0;

    double roce_tmp;
    double sumROCE = 0.0;
    double sfdce_tmp;

    double boxCoxQs;
    double boxCoxQo;

    int model, data; //Indices

    int N = periodLength-365;

    //Reinitialize the arrays for the ROCE calculations to zero
    for (int i=0; i<objs->nYears; i++)
    {
        objs->sumPrecip[i]  = 0.0;
        objs->sumQs[i]      = 0.0;
        objs->sumQo[i]      = 0.0;
        objs->meanPrecip[i] = 0.0;
        objs->meanQs[i]     = 0.0;
        objs->meanQo[i]     = 0.0;
        objs->roceAnnual[i] = 0.0;
    }

    //Arrays to hold the sorted time series
    double *sortedQs;
    sortedQs = new double [N];
    double *sortedQo;
    sortedQo = new double [N];

    //Fill out the annual summations for ROCE
    model = 365;
    data = 365+dataBegin;
    for (int i=0; i<objs->nYears; i++)
    {
        for (int j=0; j<365; j++)
        {
            //Sum of precip, Qs, and Qo
            objs->sumPrecip[i] += precip[data];
            objs->sumQs[i] += Qs[model];
            objs->sumQo[i] += Qo[data];
            model++;
            data++;
        }
    }

    //Copy the Qs and Qo arrays into sorted
    for(int i=365; i<periodLength; i++)
    {
        model = i;
        //Note: Don't subtract 1 becuase dataBegin is already zero based
        data = i+dataBegin;
        //Copy the Qs and Qo arrays into sorted
        sortedQs[i-365] = Qs[model];
        sortedQo[i-365] = Qo[data];
        //Sum of Qs and Qo
        sumDiffQs += (Qs[model]-Qo[data]);
        sumDiffQsSqr += (Qs[model]-Qo[data])*(Qs[model]-Qo[data]);
        //Box-Cox transforms with Lambda = 0.3
        boxCoxQs = (pow(Qs[model],0.3)-1.0)/0.3;
        boxCoxQo = (pow(Qo[data],0.3)-1.0)/0.3;
        tSumDiffQsSqr += (boxCoxQs-boxCoxQo)*(boxCoxQs-boxCoxQo);
    }

    //Sort them
    quickSort(sortedQs, N, -1);
    quickSort(sortedQo, N, -1);

    //Calculate the annual means for ROCE
    for (int i=0; i<objs->nYears; i++)
    {
        objs->meanPrecip[i] = objs->sumPrecip[i]/365;
        objs->meanQs[i]     = objs->sumQs[i]/365;
        objs->meanQo[i]     = objs->sumQo[i]/365;
        objs->roceAnnual[i] = (objs->meanQs[i]-objs->meanQo[i])/objs->meanPrecip[i];
        sumROCE += objs->roceAnnual[i];
    }

    //Calculations for NSE
    double sumDiffQoMeanSq = 0;

    for(int i=365; i<periodLength; i++)
    {
        data = i+dataBegin;
        sumDiffQoMeanSq += (Qo[data]-meanQoAll)*(Qo[data]-meanQoAll);
    }

    //Calculate RMSE
    objs->rmse[member]  = pow((sumDiffQsSqr/N),0.5);
    //Calculate tRMSE
    objs->trmse[member] = pow((tSumDiffQsSqr/N), 0.5);

    //Calculate the ROCE - we are now using the ROCE based on annual averages
    roce_tmp = sumROCE/objs->nYears;

    //if negative
    if (roce_tmp < 0.0)
    {
        objs->act_roce[member] = -1.0;
        //For log transform - If close to (-) zero
        /*if (roce_tmp > -1.0E-14)
        {
            objs->act_roce[member] = -1.0;
            objs->lroce[member] = -14.0;
        }
        else
        {
            objs->act_roce[member] = -1.0;
            objs->lroce[member] = log10(fabs(roce_tmp));
        }*/
    }
    //if positive
    else if (roce_tmp >= 0.0)
    {
        objs->act_roce[member] = 1.0;
        //For log transform - If close to (+)zero
        /*if (roce_tmp < 1.0E-14)
        {
            objs->act_roce[member] = 1.0;
            objs->lroce[member] = -14.0;
        }
        else
        {
            objs->act_roce[member] = 1.0;
            objs->lroce[member] = log10(roce_tmp);
        }*/
    }

    //Calculate  the SFDCE
    int lower = int(0.33*N)-1;
    int upper = int(0.67*N)-1;
    //sfdce_tmp = ((sortedQs[int(0.67*N)-1]-sortedQs[int(0.33*N)-1])-(sortedQo[int(0.67*N)-1]-sortedQo[int(0.33*N)-1]))/0.34;
    //This calculates SFDCE as a percent error ranging between 0.0 and 1.0.
    sfdce_tmp = ((sortedQs[upper]-sortedQs[lower])-(sortedQo[upper]-sortedQo[lower]))/(sortedQo[upper]-sortedQo[lower]);
    
    //If negative
    if (sfdce_tmp < 0.0)
    {
        objs->act_sfdce[member] = -1.0;
        //For log transform - If close to (-)zero
        /*if (sfdce_tmp > -1.0E-14)
        {
            objs->act_sfdce[member] = -1.0;
            objs->lsfdce[member] = -14.0;
        }
        else
        {
            objs->act_sfdce[member] = -1.0;
            objs->lsfdce[member] = log10(fabs(sfdce_tmp));
        }*/
    }
    //If positive
    else if (sfdce_tmp > 0.0)
    {
        objs->act_sfdce[member] = 1.0;
        //For log transform - If close to (+)zero
        /*if (sfdce_tmp < 1.0E-14)
        {
            objs->act_sfdce[member] = 1.0;
            objs->lsfdce[member] = -14.0;
        }
        else
        {
            objs->act_sfdce[member] = 1.0;
            objs->lsfdce[member] = log10(sfdce_tmp);
        }*/
    }
    
    //And the regular ROCE and SFDCE
    objs->roce[member]  = fabs(roce_tmp);
    objs->sfdce[member] = fabs(sfdce_tmp);

    //Calculate NSE (defined as 1.0 - Traditional NSE so that it now becomes a minimization objective)
    objs->nse[member] = 1.0-(1.0-sumDiffQsSqr/sumDiffQoMeanSq);

    //Clean up
    delete[] sortedQs;
    delete[] sortedQo;

    return;
}

void allocateObjectives(Objectives *objs, int size, int periodLength)
{
    //Allocate according to ensemble size
    objs->rmse  = new double [size];
    objs->trmse = new double [size];
    objs->roce  = new double [size];
    objs->sfdce = new double [size];
    //objs->lroce  = new double [size];
    //objs->lsfdce = new double [size];
    objs->nse   = new double [size];
    objs->act_roce  = new double [size];
    objs->act_sfdce = new double [size];

    objs->ensembleSize = size;

    //This is just an approximation that doesn't take into account the occurance of leap years
    objs->nYears = int(double(periodLength-365)/365.0);

    //Allocate some stuff
    objs->sumPrecip  = new double [objs->nYears];
    objs->sumQs      = new double [objs->nYears];
    objs->sumQo      = new double [objs->nYears];
    objs->meanPrecip = new double [objs->nYears];
    objs->meanQs     = new double [objs->nYears];
    objs->meanQo     = new double [objs->nYears];
    objs->roceAnnual = new double [objs->nYears];

    return;
}

#endif //HYMOD

