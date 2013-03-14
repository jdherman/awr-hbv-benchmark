#ifdef HBVMOD

#include "HBV.h"


//******************************************************************************************
//This just sets up the simplest configuration for HBV with one catchment, and one zone
//******************************************************************************************
void simpleConfiguration(HBV *hbv)
{

    hbv->config.nodes = 1;

    hbv->config.nodeTo = new int    [hbv->config.nodes];
    hbv->config.ak     = new double [hbv->config.nodes];
    hbv->config.xmu    = new double [hbv->config.nodes];

    hbv->config.nodeTo[0] = 1;

    hbv->config.noise = 1;

    hbv->config.nob = new int [hbv->config.noise];
    hbv->config.nob[0] = 1;

    hbv->config.nCatch = 1;

    hbv->config.nFloTo = new int    [hbv->config.nCatch];
    hbv->config.nSub   = new int    [hbv->config.nCatch];
    hbv->config.maxbas = new int    [hbv->config.nCatch];
    hbv->config.art    = new double [hbv->config.nCatch];
    hbv->config.hl1    = new double [hbv->config.nCatch];
    hbv->config.ck0    = new double [hbv->config.nCatch];
    hbv->config.ck1    = new double [hbv->config.nCatch];
    hbv->config.ck2    = new double [hbv->config.nCatch];
    hbv->config.cper   = new double [hbv->config.nCatch];
    hbv->config.perc   = new double [hbv->config.nCatch];
    hbv->config.ekk    = new double [hbv->config.nCatch];
    hbv->config.stw1   = new double [hbv->config.nCatch];
    hbv->config.stw2   = new double [hbv->config.nCatch];
    hbv->config.sums1  = new double [hbv->config.nCatch];
    hbv->config.sums2  = new double [hbv->config.nCatch];

    hbv->config.nFloTo[0] = 1;
    hbv->config.nSub  [0] = 1;

    hbv->config.maxbas[0] = 0;
    hbv->config.art   [0] = 0.0;
    hbv->config.hl1   [0] = 0.0;
    hbv->config.ck0   [0] = 0.0;
    hbv->config.ck1   [0] = 0.0;
    hbv->config.ck2   [0] = 0.0;
    hbv->config.cper  [0] = 0.0;
    hbv->config.perc  [0] = 0.0;
    hbv->config.ekk   [0] = 0.0;
    hbv->config.stw1  [0] = 0.0;
    hbv->config.stw2  [0] = 0.0;
    hbv->config.sums1 [0] = 0.0;
    hbv->config.sums2 [0] = 0.0;

    //2D Arrays
    hbv->config.ar      = new double* [hbv->config.nCatch];
    hbv->config.degw    = new double* [hbv->config.nCatch];
    hbv->config.degd    = new double* [hbv->config.nCatch];
    //hbv->config.pwp     = new double* [hbv->config.nCatch];
    hbv->config.lp      = new double* [hbv->config.nCatch];
    hbv->config.fcap    = new double* [hbv->config.nCatch];
    hbv->config.beta    = new double* [hbv->config.nCatch];
    hbv->config.sowat   = new double* [hbv->config.nCatch];
    hbv->config.pwpini  = new double* [hbv->config.nCatch];
    hbv->config.fcapini = new double* [hbv->config.nCatch];
    hbv->config.sdep    = new double* [hbv->config.nCatch];
    hbv->config.ldep    = new double* [hbv->config.nCatch];
    hbv->config.sfcf    = new double* [hbv->config.nCatch];
    hbv->config.cfr     = new double* [hbv->config.nCatch];
    hbv->config.cwh     = new double* [hbv->config.nCatch];
    hbv->config.ttlim   = new double* [hbv->config.nCatch];
    hbv->config.ttint   = new double* [hbv->config.nCatch];
    hbv->config.pdep    = new double* [hbv->config.nCatch];
    hbv->config.tcel    = new double* [hbv->config.nCatch];

    hbv->config.ar[0]      = new double [hbv->config.nSub[0]];
    hbv->config.degw[0]    = new double [hbv->config.nSub[0]];
    hbv->config.degd[0]    = new double [hbv->config.nSub[0]];
    //hbv->config.pwp[0]     = new double [hbv->config.nSub[0]];
    hbv->config.lp[0]      = new double [hbv->config.nSub[0]];
    hbv->config.fcap[0]    = new double [hbv->config.nSub[0]];
    hbv->config.beta[0]    = new double [hbv->config.nSub[0]];
    hbv->config.sowat[0]   = new double [hbv->config.nSub[0]];
    hbv->config.pwpini[0]  = new double [hbv->config.nSub[0]];
    hbv->config.fcapini[0] = new double [hbv->config.nSub[0]];
    hbv->config.sdep[0]    = new double [hbv->config.nSub[0]];
    hbv->config.ldep[0]    = new double [hbv->config.nSub[0]];
    hbv->config.sfcf[0]    = new double [hbv->config.nSub[0]];
    hbv->config.cfr[0]     = new double [hbv->config.nSub[0]];
    hbv->config.cwh[0]     = new double [hbv->config.nSub[0]];
    hbv->config.ttlim[0]   = new double [hbv->config.nSub[0]];
    hbv->config.ttint[0]   = new double [hbv->config.nSub[0]];
    hbv->config.pdep[0]    = new double [hbv->config.nSub[0]];
    hbv->config.tcel[0]    = new double [hbv->config.nSub[0]];

    //Initialize
    hbv->config.ar[0][0]      = 0.0;
    hbv->config.degw[0][0]    = 0.0;
    hbv->config.degd[0][0]    = 0.0;
    //hbv->config.pwp[0][0]     = 0.0;
    hbv->config.lp[0][0]     = 0.0;
    hbv->config.fcap[0][0]    = 0.0;
    hbv->config.beta[0][0]    = 0.0;
    hbv->config.sowat[0][0]   = 0.0;
    hbv->config.pwpini[0][0]  = 0.0;
    hbv->config.fcapini[0][0] = 0.0;
    hbv->config.sdep[0][0]    = 0.0;
    hbv->config.ldep[0][0]    = 0.0;
    hbv->config.sfcf[0][0]    = 0.0;
    hbv->config.cfr[0][0]     = 0.0;
    hbv->config.cwh[0][0]     = 0.0;
    hbv->config.ttlim[0][0]   = 0.0;
    hbv->config.ttint[0][0]   = 0.0;
    hbv->config.pdep[0][0]    = 0.0;
    hbv->config.tcel[0][0]    = 0.0;

    //3D Arrays
    hbv->config.cef   = new double** [12];
    hbv->config.atemp = new double** [12];
    hbv->config.poev  = new double** [12];

    for (int i=0; i<12; i++) 
    {
        hbv->config.cef[i]   = new double* [hbv->config.nCatch];
        hbv->config.atemp[i] = new double* [hbv->config.nCatch];
        hbv->config.poev[i]  = new double* [hbv->config.nCatch];

        hbv->config.cef[i][0]   = new double [hbv->config.nSub[0]];
        hbv->config.atemp[i][0] = new double [hbv->config.nSub[0]];
        hbv->config.poev[i][0]  = new double [hbv->config.nSub[0]];
    }

    //Initialize
    for (int i=0; i<12; i++) 
    {
        hbv->config.cef[i][0][0]   = 0.0;
        hbv->config.atemp[i][0][0] = 0.0;
        hbv->config.poev[i][0][0]  = 0.0;
    }

    return;

}

//****************************************************************************
//Read parameters
//****************************************************************************
void initParameters(HBV *hbv)
{
    //int idum;
    //int id1, id2;
    //double tdt, tt;
    //double dd, s1, s2, skk;
    //ifstream par;

    //Time step
    hbv->config.tst = 24; //tst enters as [hrs]

    hbv->config.nagg = int(24.0/hbv->config.tst + 0.1);
    hbv->config.tst *= 3600.0; //Now, tst is [s]

    //Allocate
    //I think these are allocated correctly but am not 100% sure
    hbv->flows.Qtot = new double* [hbv->config.nodes];
    hbv->flows.Qtou = new double* [hbv->config.nodes];
    hbv->flows.Qind = new double* [hbv->config.nodes];
    hbv->flows.Qout = new double* [hbv->config.nodes];
    hbv->flows.Qtr  = new double* [hbv->config.nodes];

    for (int i=0; i<hbv->config.nodes; i++) 
    {
        hbv->flows.Qtot[i] = new double [2*hbv->config.maxbas[i]];
        hbv->flows.Qtou[i] = new double [2*hbv->config.maxbas[i]];
        hbv->flows.Qind[i] = new double [2*hbv->config.maxbas[i]];
        hbv->flows.Qout[i] = new double [2*hbv->config.maxbas[i]];
        hbv->flows.Qtr[i]  = new double [2*hbv->config.maxbas[i]];
    }

    //Initialize
    for (int i=0; i<hbv->config.nodes; i++) 
    {
        for (int j=0; j<2*hbv->config.maxbas[i]; j++)
        {
            hbv->flows.Qtot[i][j] = 0.0;
            hbv->flows.Qtou[i][j] = 0.0;
            hbv->flows.Qind[i][j] = 0.0;
            hbv->flows.Qout[i][j] = 0.0;
            hbv->flows.Qtr[i][j]  = 0.0;
        }
    }

    for (int i=0; i<hbv->config.nCatch; i++)
    {
        hbv->config.ekk[i] = 1.0;
    }

    //par.close();

    return;
}

//This function reinitializes the arrays that depend on maxbas
void reinitForMaxBas(HBV *hbv)
{

    for (int i=0; i<hbv->config.nodes; i++) 
    {
        delete[] hbv->flows.Qtot[i];
        delete[] hbv->flows.Qtou[i];
        delete[] hbv->flows.Qind[i];
        delete[] hbv->flows.Qout[i];
        delete[] hbv->flows.Qtr[i];
    }

    for (int i=0; i<hbv->config.nodes; i++) 
    {
        hbv->flows.Qtot[i] = new double [2*hbv->config.maxbas[i]];
        hbv->flows.Qtou[i] = new double [2*hbv->config.maxbas[i]];
        hbv->flows.Qind[i] = new double [2*hbv->config.maxbas[i]];
        hbv->flows.Qout[i] = new double [2*hbv->config.maxbas[i]];
        hbv->flows.Qtr[i]  = new double [2*hbv->config.maxbas[i]];
    }

    //Initialize
    for (int i=0; i<hbv->config.nodes; i++) 
    {
        for (int j=0; j<2*hbv->config.maxbas[i]; j++)
        {
            hbv->flows.Qtot[i][j] = 0.0;
            hbv->flows.Qtou[i][j] = 0.0;
            hbv->flows.Qind[i][j] = 0.0;
            hbv->flows.Qout[i][j] = 0.0;
            hbv->flows.Qtr[i][j]  = 0.0;
        }
    }

    return;
}



#endif