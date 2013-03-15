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

#include "QuickSort.h"

template<class T>
inline void SWAP(T &a, T &b)
    {T dum=a; a=b; b=dum;}

inline int MIN(const int &a, const int &b)
        {return b < a ? (b) : int(a);}

//********************************************************************************
//Quick Sort routine from Numerical Recipes ed. 3 by Press et al. (2007)
//********************************************************************************

void quickSort(double *arr, int size, int m=-1)
{
    static const int M=7, NSTACK=64;
    int i,ir,j,k,jstack=-1,l=0,n=size;
    double a;

    int *istack = new int[NSTACK];

    if (m>0) n = MIN(m,n);
    ir=n-1;
    for (;;) {
        if (ir-l < M) {
            for (j=l+1;j<=ir;j++) {
                a=arr[j];
                for (i=j-1;i>=l;i--) {
                    if (arr[i] <= a) break;
                    arr[i+1]=arr[i];
                }
                arr[i+1]=a;
            }
            if (jstack < 0) break;
            ir=istack[jstack--];
            l=istack[jstack--];
        } else {
            k=(l+ir) >> 1;
            SWAP(arr[k],arr[l+1]);
            if (arr[l] > arr[ir]) {
                SWAP(arr[l],arr[ir]);
            }
            if (arr[l+1] > arr[ir]) {
                SWAP(arr[l+1],arr[ir]);
            }
            if (arr[l] > arr[l+1]) {
                SWAP(arr[l],arr[l+1]);
            }
            i=l+1;
            j=ir;
            a=arr[l+1];
            for (;;) {
                do i++; while (arr[i] < a);
                do j--; while (arr[j] > a);
                if (j < i) break;
                SWAP(arr[i],arr[j]);
            }
            arr[l+1]=arr[j];
            arr[j]=a;
            jstack += 2;
            if (jstack >= NSTACK) throw("NSTACK too small in sort.");
            if (ir-i+1 >= j-l) {
                istack[jstack]=ir;
                istack[jstack-1]=i;
                ir=j-1;
            } else {
                istack[jstack]=j-1;
                istack[jstack-1]=l;
                l=i;
            }
        }
    }

    //Clean up
    delete[] istack;

    return;
}


