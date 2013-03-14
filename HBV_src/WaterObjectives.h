#if defined(HYMOD) || defined(HBVMOD) || defined(HLRMS)

#ifndef __objectives_h
#define __objectives_h

//Objectives data structure
struct Objectives
{

    int ensembleSize;

    //int calcMean;     //Flag for calculate the mean
    //int calcBias;     //Flag for calculating the bias
    //int calcHMLE;     //Flag for calculating Heteroskedastic Maximum Liklihood Estimator
    //int calcNash;     //Flag for calculating Nash-Sutcliffe Efficiency
    //int calcVar;      //Flag for calculating variance
    //int calcDV;       //Flag for calculating ??

    //int calcRMSE;       //Flag for calculating Root Mean Squared Error
    //int calcTRMSE;      //Flag for calculating Box-Cox transformed Root Mean Squared Error
    //int calcROCE;       //Flag to calculate runoff coefficient error
    //int calcSFDCE;      //flag to calcualte the slope of the flow duration curve error

    //double *mean;
    //double *bias;
    //double *hmle;
    //double *nash;
    //double *var;
    //double *dv;

    int nYears; 

    double *rmse;       //Root mean squared error
    double *trmse;      //Box cos transformed root mean squared error
    double *roce;       //Runoff coefficient error absolute value
    double *sfdce;      //Slope of the flow duration curve error absolute 
    //double *lroce;      //Log transformed runoff coefficient error absolute value
    //double *lsfdce;     //Log transformed slope of the flow duration curve error absolute value
    double *nse;        //Nash-Sutcliffe Efficiency

    double *act_roce;   //Flag to indicate the sign of the actual ROCE
    double *act_sfdce;  //Flag to indicate the sign of the actual SFDCE

    //Pointer arrays related to the calculation of ROCE
    double *sumPrecip;
    double *sumQs;
    double *sumQo;

    double *meanPrecip;
    double *meanQs;
    double *meanQo;

    double *roceAnnual;

};

void allocateObjectives(Objectives *objs, int size, int periodLength);
void calculateObjectives(Objectives *objs, double *Qs,double *Qo, double *precip, int dataBegin, int periodLength, int member);


#endif

#endif //HYMOD
