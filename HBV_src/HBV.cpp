#ifdef HBVMOD

#include "HBV.h"

extern HBV hbv;
extern HBV* hbvPtr;
extern int lolo;
extern int oldSnow;

//***********************************************************************
//Snowmelt routine
//***********************************************************************
void snow(double &sd, double tc, double pd, double dd, double &pe, double tt, double tdt)
{
    ///////////////////////////////////////////////////////////
    //Parameter in code | parameter in manual | description
    ///////////////////////////////////////////////////////////
    //sd  | NA   | snow storage depth
    //tc  | T    | current temperature
    //pd  | RF   | rainfall
    //dd  | NA   | degree day snow factor [mm/(degC*day)]
    //pe  | NA   | potential evapotranspiration
    //tt  | tt   | theshold temperature which is usually close to 0 deg C - defines the temperature above which snow melt occurs
    //tdt | degw | ??

    //////////
    //Notes
    //////////
    //No glacier melt in this version of the code
    //No dttm parameter to create a mixture of rain and snow over a certain temperature range
    //No refreezing of melt water

    double padd;
    double smelt;

    ///////////////////////////////////////////////////////////
    //Variable in code | parameter in manual | description
    ///////////////////////////////////////////////////////////
    //padd  | NA | ??
    //smelt | NA | snow melt = cfmax*(T-tt)
    //cfmax | NA | melting factor = dd*padd

    //If current temperature is greater than the snow melt temperature threshold...
    if (tc > tt)
    {
        //If we are more than 1 deg above, then we calculate some sort of factor padd??
        if (tc > tt + 1.0) padd = 1.0 + pd * tdt;
        else padd = 1.0;

        //If there is actually snow to melt in the snow store...
        if (sd > 0.0)
        {
            //Calculate snow melt using this mystery factor padd
            smelt = (tc - tt) * dd * padd;
            //If snow melt that wants to occur is more than what is actually stored...
            if (smelt > sd)
            {
                //Potential evap is precip depth and snow depth together
                pe = pd + sd;
                //All of the snow has melted
                sd = 0.0;
            }
            //Otherwise, we melt a portion of the snow store
            else
            {
                //Potential evap is precip together with what acutally melted
                pe = pd + smelt;
                //Remove the amount that melted from the snow store
                sd -= smelt;
            }
        }
        //If there is no snow to melt, than the potential evap is the precip.
        else pe = pd;
    }
    //Otherwise, just add the precip to the snow store and set potential evap to 0
    else
    {
        sd += pd;
        pe = 0.0;
    }

    return;
}

//***********************************************************************
//My modified snowmelt routine
//***********************************************************************
void mySnow(double &sd, double &ld, double sfcf, double cfr, double cwh, double tc, double pd, double dd, double &pe, double tt, double tti)
{
    ///////////////////////////////////////////////////////////
    //Parameter in code | parameter in manual | description
    ///////////////////////////////////////////////////////////
    //sd  | NA   | snow storage depth
    //tc  | T    | current temperature
    //pd  | RF   | rainfall
    //dd  | NA   | degree day snow factor [mm/(degC*day)]
    //pe  | NA   | potential evapotranspiration
    //tt  | tt   | theshold temperature which is usually close to 0 deg C - defines the temperature above which snow melt occurs

    //Added:
    //sfcf | snowfall correction factor
    //cfr  | refreezing coefficient
    //cwh  | fraction of snow store capable of holding liquid water
    //ld   | the depth of the liquid portion of the snow store

    //////////
    //Notes
    //////////
    //No glacier melt in this version of the code
    //No dttm parameter to create a mixture of rain and snow over a certain temperature range

    double smelt = 0.0;
    double refreeze = 0.0;
    double ldMax = 0.0; //The new size of the liquid water portion of the snow store based on the current size of the snow store

    double snow, snowFrac;
    double rain;

    //ofstream out;
    //out.open("Snow.out", ios_base::app);

    ///////////////////////////////////////////////////////////
    //Variable in code | parameter in manual | description
    ///////////////////////////////////////////////////////////
    //smelt | NA | snow melt = cfmax*(T-tt)
    //refreeze | the amount of liquid that refreezes in the snow pack
    //ldMax | the maximum capacity of the liquid store in the snow pack
    //snowFrac | the fraction of the precip at the current temperature falling as snow
    //snow | the actual amount of snow falling at the current temperature
    //rain | the actual amount of rain falling at the current temperature

    //Calculate the fraction of rain and snow

    //All rain, no snow
    if (tc >= tt+tti)
    {
        rain = pd;
        snow = 0.0;
    }
    //All snow, no rain
    else if (tc <= tt-tti)
    {
        snow = pd;
        rain = 0.0;
    }
    //Linear mixture of snow and rain in interval +-tti
    else 
    {
        snowFrac = 1.0 - (1.0/(2.0*tti))*(tc-(tt-tti));
        snow = pd*snowFrac;
        rain = pd*(1-snowFrac);
    }

    //Initialize effective precip to 0
    pe = 0.0;

    //If there is actually snow to melt in the snow store...
    if (sd > 0.0)
    {
        //Calculate melt
        smelt = (tc - tt)*dd;

        //Fill ld with smelt and rain first

        //If the melt is greater than the snow that is stored
        if (smelt > sd)
        {
            //Add all of the snow and incoming rain to the liquid sotre
            ld += sd + rain;
            sd = 0.0;
        }
        //Otherwise...
        else
        {
            //Just add the melt portion and the incoming rain
            ld += smelt + rain;
            //And decrease the snow store accordingly
            sd -= smelt;
        }

        //Now, calculate how much liquid can actually be held given the remaining snow
        ldMax = sd*cwh;

        //If we just filled it past capacity...
        if (ld > ldMax)
        {
            pe += (ld - ldMax);
            ld = ldMax;
        }
        //If it held everything, then there is no excess
        else pe += 0.0;
    }
    //If there is no snow to melt, than the effective precip is the precip.
    else pe += rain;

    //There is a slight conceptual problem here...
    //If there is a mix of rain and snow, does the incoming snow at this time step contribute to snow melt in this time step?
    //Right now, we assume it doesn't (i.e., we calculate snow melt first, and then add incoming snow and refreeze to the snow store).

    //Calculate the amount of water that will potentially refreeze
    refreeze = (tt - tc)*dd*cfr;
    //If the amount that will refeeze is greater than the liquid content of the snow...
    if (refreeze > ld)
    {
        //Add the entire liquid portion of the snow store to the sd
        sd += ld;
        ld = 0.0;
    }
    //If there is more liquid than will actually refreeze...
    if (refreeze < ld)
    {
        //Just add the refreeze portion to the snow store
        sd += refreeze;
        //And remove it from the liquid store
        ld -= refreeze;
    }
    //Here, we are multiply the precipitation fraction that is snow by the snowfall correction factor
    //And add it to the snow store
    sd += snow*sfcf;

    //Now, calculate how much liquid can actually be held given the snow store - not used here but useful for output
    ldMax = sd*cwh;

    //And there is no effective precip
    pe += 0.0;

    //Old way
    //If current temperature is greater than the snow melt temperature threshold...
    /*if (tc > tt)
    {
        //If there is actually snow to melt in the snow store...
        if (sd > 0.0)
        {
            //Calculate melt
            smelt = (tc - tt)*dd;

            //Fill ld with smelt and pd first

            //If the melt is greater than the snow that is stored
            if (smelt > sd)
            {
                //Add all of the snow to the liquid sotre
                ld += sd + pd;
                sd = 0.0;
            }
            //Otherwise...
            else
            {
                //Just add the melt portion and the precip
                ld += smelt + pd;
                //And decrease the snow store accordingly
                sd -= smelt;
            }

            //Now, calculate how much liquid can actually be held given the remaining snow
            ldMax = sd*cwh;

            //If we just filled it past capacity...
            if (ld > ldMax)
            {
                pe = ld - ldMax;
                ld = ldMax;
            }
            //If it held everything, then there is no excess
            else pe = 0.0;
        }
        //If there is no snow to melt, than the effective precip is the precip.
        else pe = pd;
    }
    //Otherwise, just add the precip to the snow store and set potential evap to 0
    else
    {
        //Calculate the amount of water that will refreeze
        refreeze = (tt - tc)*dd*cfr;
        //If the amount that will refeeze is greater than the liquid content of the snow...
        if (refreeze > ld)
        {
            //Add the entire liquid portion of the snow store to the sd
            sd += ld;
            ld = 0.0;
        }
        //If there is more liquid than will actually refreeze...
        if (refreeze < ld)
        {
            //Just add the refreeze portion to the snow store
            sd += refreeze;
            //And remove it from the liquid store
            ld -= refreeze;
        }
        //Here, we are multiply the precipitation by the snowfall correction factor
        //And add it to the snow store
        sd += pd*sfcf;

        //Now, calculate how much liquid can actually be held given the snow store - not used here but useful for output
        ldMax = sd*cwh;

        //And there is no effective precip
        pe = 0.0;
    }*/

    //out << tc << "  " << tt << "  " << pd << "  " << sd << "  " << ld << "  " << ldMax << "  " << smelt << "  " << refreeze << "  " << pe << endl;

    return;
}

//*****************************************************************************
//Subroutine calculates snow accumulation and snowmelt
//*****************************************************************************
void soil(double pe, double &sw, double fc, double be, double &qd)
{
    ///////////////////////////////////////////////////////////
    //Parameter in code | parameter in manual | description
    ///////////////////////////////////////////////////////////
    //be | beta    | emperical coefficient which controls the contribution to the response function delQ/delP
    //fc | fc      | maximum soil moisture storage (in mm)
    //pe | NA      | effective precipitation - that which is not stored as snow...
    //qd | NA      | runoff depth
    //sw | sm      | computed soil moisture storage

    double help;
    double hh;
    double hsw;
    double swo = sw;

    ///////////////////////////////////////////////////////////
    //Variable in code | parameter in manual | description
    ///////////////////////////////////////////////////////////
    //help | delQ/delP   | runoff coefficient = (sm/fc)^beta
    //hh   | NA          | difference between initial soil moisture storage and computed soil moisture storage
    //hsw  | 1-delQ/delP | increase in soil moisture storage 1-delQ/delP
    //swo  | NA          | initial soil moisture storage
    
    //If the soil moisture storage is already at capacity...all precip runs off
    if (sw >= fc) qd = pe; //
    //Otherwise...
    else
    {
        //In Fortran code but was being replaced by next line
        //help = sw / fc;
        //help = pow(fabs((help - 0.4)/0.6),be);

        //Calculate the runoff coefficient delQ/delP
        help = pow((sw/fc),be);
        //This is the portion of the effective precip that goes into storage
        hsw = pe * (1.0 - help);

        //If the amount going into the soil moisture storage will result in exceeding the capacity of the store...
        if ((hsw + sw) > fc)
        {
            //The runoff depth is effective precip - (max storage - soil moisture)
            qd = pe - fc + sw;
            //We are at capacity
            sw = fc;
        }
        //Otherwise, the evapotranspiration will be less than the potential by hsw
        else
        {
            //Runoff depth is effective precip - the portion that goes into storage
            qd = pe - hsw;
            //We add the portion that goes into storage into the storage
            sw += hsw;
        }
    }

    hh = fabs(sw - swo);

    if (hh > 10.6) lolo = 1;

    return;
}

//*****************************************************************************
//Evapotranspiration routine
//*****************************************************************************
void evapNew(double &sw, double tc, double fcap, double lp, double avt, double pev, double cfa, double &etp)
{

    ///////////////////////////////////////////////////////////
    //Parameter in code | parameter in manual/lit | description
    ///////////////////////////////////////////////////////////
    //sw  | sm  | soil moisture storage
    //tc  | T   | current temperature
    //pw  | PWP | limiting soil moisture at which potential evapotranspiration takes place - no longer used
    //avt | Tm  | average monthly air temp.
    //pev | PEm | average monthly potential evapotranspiration
    //cfa | C   | coefficient related to ETP change
    //etp | Ea  | actual evapotranspiration
    //fcap|     | capcity of soil moisture store
    //lp  |     | threshold of soil moisture store for potential evap

    double ult;

    ///////////////////////////////////////////////////////////
    //Variable in code | variable in manual/lit | description
    ///////////////////////////////////////////////////////////
    //ult | SM/PWP | ratio between soil moisture and limiting soil moisture for potential ETP

    //Calculate the adjusted potential evapotranspiration - PEa - this later gets changed to actual ETP
    //etp = (1.0 + cfa * (tc - avt)) * pev;
    //Removed correction factor - now correction is made to PE before it comes into this routine
    etp = pev;

    //ult = 1.0;

    //If the amount of moisture in the soil store is less than the limiting soil moisture for potential ETP...
    //if (sw < pw) ult = sw / pw; //ult is the ratio between the two

    ult = min(sw/(fcap*lp),1.0); //Now, we are calculating this factor more consistant with what it seen in the literature

    //Now, etp is the actual evapotranspiration since we've mulitplied by ult.
    etp *= ult;

    //If etp ends up being negative, just set to 0
    if (etp < 0.0) etp = 0.0;

    //If there is enough in the soil moisture store to supply the etp, subtract it out
    if (sw > etp) sw -= etp;
    //Otherwise...
    else
    {
        //etp is jsut whatever is available in the soil moisture store
        etp = sw - 0.0001;
        //And there is nothing left after etp
        sw = 0.0001;
    }

    return;
}

//****************************************************************************
//Calculates soil, snow and evapo for each zone
//****************************************************************************
void zonesNew(HBV *hbv, int ic, int dataIndex)
{
    Config *config = &hbv->config;
    //Stats  *stats  = &hbv->stats;
    MOPEXData *data = &hbv->data;

    double sw, sd, tc, pd, dd, tdt, tt, tti;
    double ld, sfcf, cfr, cwh; //Parameters I added for my snow model
    //double hlo, ase;
    double be, fc;
    //double qds;
    double pe;
    double qd;
    //double pw;
    double lp, fcap, avt, cfa, pev;
    double etp;

    //if (stats->ismo != imon)
    //{
    //    stats->ismo = imon;
    //    stats->iamo++;
    //}

    for (int i=0; i<config->nSub[ic]; i++)
    {
        if (config->ar[ic][i] <= 0.0) continue;

        //Actual evap??
        //stats->ess[ic][stats->iamo] += config->pdep[ic][i] * config->ar[ic][i] / config->art[ic];
        
        sw  = config->sowat[ic][i];  //Soil moisture storage
        sd  = config->sdep[ic][i];   //Snow storage
        tc  = config->tcel[ic][i];   //Current temperature
        pd  = config->pdep[ic][i];   //Raw precipitation depth
        dd  = config->degd[ic][i];   //Degree day factor
        tdt = config->degw[ic][i];   //??
        tt  = config->ttlim[ic][i];  //Temperature threshold below which snow occurs
        ld  = config->ldep[ic][i];   //Liquid storage inside snow store
        sfcf = config->sfcf[ic][i];  //snow fall correction factor
        cfr = config->cfr[ic][i];    //Refreeze coefficient
        cwh = config->cwh[ic][i];    //Fraction of snow store that can hold water
        tti = config->ttint[ic][i];  //Temperate interval over which a mix of snow and rain occurs

        //Snow module
        if (oldSnow) snow(sd, tc, pd, dd, pe, tt, tdt);
        else mySnow(sd, ld, sfcf, cfr, cwh, tc, pd, dd, pe, tt, tti);
        //pe - effective precip - this is what comes out of the snow model
        //sd - snow depth - is also updated in snow
        //ld - liquid storage depth - is also updated in snow

        //Loss and sealed runoff
        //hlo = config->hloss[ic][i];
        //ase = config->aseal[ic][i];
        //sw  = config->sowat[ic][i];
        //lossSeal(hlo, ase, pe, sw, qds);

        //Change storage volume
        //config->stw1[ic] += config->ar[ic][i] * qds / config->art[ic];

        config->sdep[ic][i] = sd; //snow depth
        config->ldep[ic][i] = ld; //liquid depth in snow store
        be = config->beta[ic][i]; //beta parameter
        fc = config->fcap[ic][i]; //maximum soil storage capacity

        //QD direct runoff
        if (pe > 0.6) lolo = 1;

        //Soil module
        soil(pe, sw, fc, be, qd);
        //qd - runoff from soil moisture module - is updated in soil.

        //Change in storage volume
        config->stw1[ic] += config->ar[ic][i] * qd / config->art[ic];

        //pw  = config->pwp[ic][i];          //PWP - limiting soil moisture at which potential evapotranspiration takes place
        lp = config->lp[ic][i]; //Now we are using lp instead of PWP
        //avt = config->atemp[imon][ic][i];  //Monthly average temperature
        //cfa = config->cef[imon][ic][i];    //Coefficient related to ETP change
        //pev = config->poev[imon][ic][i] / double(config->nagg); //monthly average potential evapotranspiration
        fcap  = config->fcap[ic][i];
        pev   = config->poev[0][ic][i];
        //Pull the correct PE adjustment factor for this month - MOPEX data only!
        //cfa   = data->peAdjust[data->date[dataIndex][1]-1]; 

        //Evapotranspiration module
        evapNew(sw, tc, fcap, lp, -1, pev, -1, etp);
        //sw  - soil moisture is updated in evap
        //etp - actual evapotranspiration is updated in evap

        //Actual evapotranspiration
        //stats->evp[ic][stats->iamo] += etp * config->ar[ic][i] / config->art[ic];
        //Soil moisture store
        config->sowat[ic][i] = sw;

    } //loop through nSub

    return;

}

//*****************************************************************************
//Calculate discharge originating from subcatchment IC
//*****************************************************************************
void disch(HBV *hbv, int ic, double &Qall)
{
    ///////////////////////////////////////////////////////////
    //Parameter in code | parameter in manual/lit | description
    ///////////////////////////////////////////////////////////
    //Qall | Q0+Q1+Q2 | Total discharge from both reservoirs

    Config *config = &hbv->config;

    double Q0, Q1, Q2;
    double perc;

    ///////////////////////////////////////////////////////////
    //Variable in code | variable in manual/lit | description
    ///////////////////////////////////////////////////////////
    //Q0 | Q0 | Near surface flow
    //Q1 | Q1 | Inter flow
    //Q2 | Q2 | Base flow
    //ck0 | k0 | Near surface flow storage constant
    //ck1 | k1 | Inter flow storage constant
    //ck2 | k2 | Base flow storage constant
    //perc | Qperc | Percolation
    //cper | kperc | Percolation storage constant
    //stw1 | S1 | Upper reservoir water level
    //stw2 | S2 | Lower reservoir water level
    //hl1  | L  | Threshold water level for near surface flow
    //ekk  | NA | Some sort of factor that controls the fraction of percolation that makes it into the lower reservoir 

    //If the upper reservoir water level is above the threshold for near surface flow
    if (config->stw1[ic] > config->hl1[ic])
    {
        //Calculate it...
        Q0 = (config->stw1[ic] - config->hl1[ic])*config->ck0[ic];
        //And move it out of the reservoir
        config->stw1[ic] -= Q0;
    }
    else Q0 = 0.0;

    //If there is still water left in the upper reservoir
    if (config->stw1[ic] > 0.0)
    {
        //Calculate what now goes into inter flow
        Q1 = config->stw1[ic] * config->ck1[ic];
        //And move this out of the reservoir as well
        config->stw1[ic] -= Q1;
    }
    else Q1 = 0.0;

    //Now calculate percolation amount
    //perc = config->stw1[ic] * config->cper[ic];
    perc = config->perc[ic]; //Change percolation to L/T instead of using a percolation coefficient

    //If there is still anough water in the upper reservois to completely supply percolation...
    if (config->stw1[ic] > perc)
    {
        //Take the percolation out of the upper reservoir
        config->stw1[ic] -= perc;
        //And put it into the lower reservoir
        config->stw2[ic] += perc * config->ekk[ic];
    }
    //Otherwise...
    else
    {
        //There is nothing left in the upper reservoir
        config->stw1[ic] = 0.0;
        //We just put what we can from the upper into the lower
        config->stw2[ic] += config->stw1[ic] * config->ekk[ic];
    }

    //If there is water in the lower reservoir...
    if (config->stw2[ic] > 0.0)
    {
        //Calculate base flow
        Q2 = config->stw2[ic] * config->ck2[ic];
        //And remove this form the lower reservoir storage
        config->stw2[ic] -= Q2;
    }
    else Q2 = 0.0;

    //Total discharge as m3/sec
    //Qall = (Q0 + Q1 + Q2) * config->art[ic] * 1000.0 / config->tst;
    //I don't know what he was doing above, but now the units should be in mm/tst;
    Qall = (Q0 + Q1 + Q2);
    if (Qall > 700.0 / double(config->nagg)) lolo = 1;

    //Track the reservoir levels
    config->sums1[ic] += config->stw1[ic];
    config->sums2[ic] += config->stw2[ic];

    return;

}

//*********************************************************************
//Transformation of the generated runoff QALL
//*********************************************************************
void outind(HBV *hbv, double Qall, int ic, int it)
{
    ///////////////////////////////////////////////////////////
    //Parameter in code | parameter in manual/lit | description
    ///////////////////////////////////////////////////////////
    //Qall | Q0+Q1+Q2 | Total discharge from both reservoirs

    Config *config = &hbv->config;
    Flows  *flows  = &hbv->flows;

    int m2;
    double wsum;
    double *wei = new double [config->maxbas[ic]];

    ///////////////////////////////////////////////////////////
    //Variable in code | variable in manual/lit | description
    ///////////////////////////////////////////////////////////
    //wei | g(t,MAXBAS) | transformation function consisting os a triangular weighting function and one free parameter
    //Qind | NA | This is the flow from the single Qall spread out over time according to the transformation function
    //Qout | NA | The final flow output by the model

    m2   = (config->maxbas[ic] / 2)-1;
    wsum =  0.0;

    //Calculate the values of the transformation function according to maxbas
    for (int i=0; i<config->maxbas[ic]; i++)
    {
        if (i <= m2) wei[i] = double(i+1);
        else wei[i] = double(config->maxbas[ic]-(i+1)) + 1.0;
        wsum += wei[i];
    }

    //Now, spread the flow Qall out over Qind according to the transformation function
    for (int i=0; i<config->maxbas[ic]; i++)
    {
        wei[i] /= wsum;
        //Qind is constantly added to by the transformed Qall.  In other words, when Qall is transformed (spread out over time)
        //it is then added to whatever currently exists in Qind for those time steps.  In other words, a previous transformation of
        //Qall for the previous time step placed flows in Qind in times that overlapped with the currently transformed flow times.
        flows->Qind[ic][it+i] += Qall * wei[i];
    }

    //Update the final Qout for this time step
    flows->Qout[ic][it] = flows->Qind[ic][it];
    flows->Qtou[ic][it] = 0.0;

    if (flows->Qout[ic][it] > 50.0) lolo = 1;

    delete[] wei;

    return;

}

//*********************************************************************
//Muskingum flood routing
//*********************************************************************
void musking(HBV *hbv, int ic, int it)
{
    Config *config = &hbv->config;
    Flows  *flows  = &hbv->flows;

    int nfc;
    int nf;
    double ts;
    double c1, c2;

    //for (int j=0; j<20; j++)
    //{
    //    if (ic == config->nFloTo[j]) nfc = j;
    //}
    
    nfc = ic;

    if (flows->Qtou[ic][it] < 0.000001)
    {
        flows->Qtou[ic][it] = flows->Qout[nfc][it];
    }

    nf = ic;
    ts = config->tst / 3600.0;

    //if (config->ak[ic] < 0.01)
    //{
    //    flows->Qout[nf][it] += flows->Qout[ic][it];
    //    return;
    //}
    //if (nf == ic) return;

    //I think that ak is the lag parameter and xmu is the damping parameter
    c1 = ts / (config->ak[ic] * (1.0 - config->xmu[ic]) + 0.5 * ts);
    c2 = 1.0 - c1 * config->ak[ic] / ts;

    if (it > 0)
    {
        flows->Qtr[nf][it] = flows->Qtr[nf][it-1] + c1 * (flows->Qtou[ic][it-1] - flows->Qtr[nf][it-1]);
        flows->Qtr[nf][it] = flows->Qtr[nf][it]   + c2 * (flows->Qtou[ic][it]   - flows->Qtou[ic][it-1]);
    }
    else flows->Qtr[nf][it] = c2 * flows->Qout[ic][it];

    nf = config->nodeTo[ic]-1;

    if (nf >= 0) //Node index
    {
        //Search for the catchment that corresponds with this node
        for (int j=0; j<hbv->config.nCatch; j++)
        {
            if (nf == config->nFloTo[j]-1) 
            {
                nfc = j; //Catchment index
                //break;
            }
        }
        if (flows->Qtou[nf][it] < 0.000001)
        {
            flows->Qtou[nf][it] = flows->Qout[nfc][it];
        }
        flows->Qtou[nf][it] += flows->Qtr[ic][it];
    }

    //Add flood routing to individual generated discahrge
    //nf = config->nodeTo[ic];

    return;

}

//*********************************************************************
//Reset discharge data
//*********************************************************************
void backflow(HBV *hbv)
{
    int klen;
    //int klen = 39;

    //This routine now seems to be working well with the dynamic array allocation.
    //The sticking point was that these arrays actually need to be larger than maxbas
    //To resolved, I made them twice as large and klen is calculated accordingly.

    for (int i=0; i<hbv->config.nodes; i++)
    {
        //Find which catchment mapps to this node
        for (int j=0; j<hbv->config.nCatch; j++)
        {
            if (hbv->config.nFloTo[j]-1 == i) 
            {
                klen = 2*hbv->config.maxbas[j]-1;
                break;
            }
        }
        for (int k=0; k<klen; k++)
        {
            hbv->flows.Qtou[i][k] = hbv->flows.Qtou[i][k+1];
            hbv->flows.Qind[i][k] = hbv->flows.Qind[i][k+1];
            hbv->flows.Qout[i][k] = hbv->flows.Qout[i][k+1];
            hbv->flows.Qtr[i][k]  = hbv->flows.Qtr[i][k+1];
        }
        hbv->flows.Qind[i][klen] = 0.0;
        hbv->flows.Qout[i][klen] = 0.0;
        hbv->flows.Qtr[i][klen]  = 0.0;
    }

      return;
}

//*********************************************************************
//Reset discharge data
//*********************************************************************
void summit(HBV *hbv, int it)
{

    for (int i=0; i<hbv->config.nodes; i++) hbv->flows.Qtot[i][it] = hbv->flows.Qtou[i][it];

    return;

}

//*************************************************************************
//Do processing
//    Snow accounting
//    Soil accounting
//    Evapotranspiration
//    Storage accounting
//    Discharge calculation
//    Flood routing
//    QOUT - Total outflow from catchment I
//    QTR - transformed outflow which left I (Muskingum)
//*************************************************************************
void processNew(HBV *hbv, int startingIndex, int PeriodLength, int writeOutput)
{

    Config *config = &hbv->config;
    Flows  *flows  = &hbv->flows;
    //Stats  *stats  = &hbv->stats;
    ofstream reser, resu, summ;

    //int nscag; //Number of statistical time scales
    //int isend, imon;
    int nhb;

    //double nsums[20][20];
    //double ntsums[20][20];
    double *Qis; //[20]
    //double erro1[20];
    //double erro2[20];
    //double avq[20];
    //double vaq[20];
    //double avm[20];
    //double vam[20];
    //double cor[20][20];
    //double bm[20][20];
    double *Qagg; //[20];
    double **ptart; //[20][200];

    double Qall;
    //double ww;

    Qis  = new double [config->nodes];
    Qagg = new double [config->nCatch];

    ptart = new double* [config->nCatch];

    for (int i=0; i<config->nCatch; i++)
    {
        ptart[i] = new double [config->nSub[i]];
    }

    if (writeOutput)
    {
        //Open the output files
        reser.open  (hbv->files.reserfileName.c_str(), ios::out);
        if (!reser)
        {
            cout << "Error opening reservoir output file: " << hbv->files.reserfileName << endl;
            exit(1);
        }
        resu.open  (hbv->files.resufileName.c_str(), ios::out);
        if (!resu)
        {
            cout << "Error opening simulated discharge output file: " << hbv->files.resufileName << endl;
            exit(1);
        }
    }

    //Zero calculated discharge
    for (int ic = 0; ic<config->nCatch; ic++)
    {
        //JBK - Changed this from 10 to what you see...
        for (int it=0; it<2*hbvPtr->config.maxbas[ic]; it++)
        {
            flows->Qout[ic][it] = 0.0;
            flows->Qind[ic][it] = 0.0;
            flows->Qtr[ic][it]  = 0.0;
        }
    }

    //int itv = 1;
    int it = 1; //Careful here...we are redefining it so make sure that we aren't using it in a loop

    for (int itv=0; itv<PeriodLength; itv++)
    {
    //Read in temperature and precip data for this time step
//C:  
        //meteo(hbv, isend, imon);

        config->tcel[0][0]    = hbvPtr->data.avgTemp[startingIndex+itv];
        config->pdep[0][0]    = hbvPtr->data.precip[startingIndex+itv];
        //config->poev[0][0][0] = hbvPtr->data.evap[startingIndex+itv];
        //Now, we are using the Hamon PE
        config->poev[0][0][0] = hbvPtr->evap.PE[itv];

        for (int i=0; i<config->nCatch; i++)
        {
            Qagg[i] = 0.0;
            for (int j=0; j<config->nSub[i]; j++)
            {
                if (config->ar[i][j] > 0.0) ptart[i][j] = config->pdep[i][j];
            }
        }

//        if (isend == 1) goto B;


        for (int iagg = 0; iagg<config->nagg; iagg++)
        {
            //nhb = (2 * config->nagg) / 4;
            nhb = config->nagg;
            for (int i=0; i<config->nCatch; i++)
            {
                for (int j=0; j<config->nSub[i]; j++)
                {
                    if (config->ar[i][j] > 0.0)
                    {
                        if (iagg >= config->nagg - nhb) config->pdep[i][j] = ptart[i][j] / double(nhb);
                        else config->pdep[i][j] = 0.0;
                    }
                }
            }
            //For each catchment
            for (int ic=0; ic<config->nCatch; ic++)
            {
                //Calculate snow, soil, and evap for each zone
                zonesNew(hbv, ic, startingIndex+itv);
                //Calculate the resulting discharge
                disch(hbv, ic, Qall);
                //Calculate subcatchment generated discharge curve
                outind(hbv, Qall, ic, it);
            }
            //We aren't using routing right now so we skip it here
            //for (int ic=0; ic<config->nodes; ic++)
            //{
            //    //Flood routing
            //    musking(hbv, ic, it);
            //}
            ////Add values Muskingam + individual discharge
            //summit(hbv, it);
            //Not doing rounting so...
            flows->Qtot[0][it] = flows->Qout[0][it];

            //Calculated aggregated discharge
            for (int iL = 0; iL<config->nodes; iL++)
            {
                Qagg[iL] += flows->Qtot[iL][it] / double(config->nagg);
            }
            it = 1;
            //This may be a problem area
            backflow(hbvPtr);
        }

        if (writeOutput)
        {
            reser.setf(ios_base::fixed);
            reser << setprecision(2);
            reser << setw(5) << itv;
            for (int ic=0; ic<config->nCatch; ic++)
            {
                reser << setw(10) << config->sums1[ic] / double (config->nagg);
                reser << setw(10) << config->sums2[ic] / double (config->nagg);
            }
            reser << endl;
        }

        for (int ic=0; ic<config->nCatch; ic++)
        {
            config->sums1[ic] = 0.0;
            config->sums2[ic] = 0.0;
        }

        //No longer needed
        //readFlow(hbv, Qis);

        //!!!!!!!!!!!!!!!!!!
        //statis(hbv, Qagg, it, imon);

        if (Qagg[0] > 300) lolo = 1;

        if (writeOutput)
        {
            resu.setf(ios_base::fixed);
            resu << setprecision(2);
            resu << setw(5) << itv;
            for (int ll=0; ll<config->nodes; ll++)
            {
                resu << setw(10) << Qagg[ll];
                //resu << setw(10) << Qis[ll];
                resu << setw(10) << hbvPtr->data.flow[startingIndex+itv];
            }
            resu << endl;
        }

        for (int ll=0; ll<config->nodes; ll++)
        {
            //Sotring Qagg in an array for objective function calculations
            hbv->flows.Qmodelled[ll][itv] = Qagg[ll];
        }

        int isdo = 0;

        //for (int ii=0; ii<hbv->seltime.nally; ii++)
        //{
        //    if (config->nity == hbv->seltime.nyear[ii])
        //    {
        //        if (config->nita >= hbv->seltime.nbtag[ii] && config->nita <= hbv->seltime.netag[ii])
        //            isdo = 1;
        //    }
        //}

        //cout << itv << endl;
        //itv++;
//    goto C;
    }

//B:
    if (writeOutput)
    {
        reser.close();
        resu.close();
    }

    delete[] Qagg;
    delete[] Qis;

    for (int i=0; i<config->nCatch; i++)
    {
        delete[] ptart[i];
    }
    delete[] ptart;

    return;
}


#endif