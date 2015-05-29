#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mixfft.h"

void twiddleFFT_odd(int sofarRadix, int radix, int remainRadix, int groupOffset,
				  double twiddleRe[], double twiddleIm[],
				  double trigRe[], double trigIm[],
				  double yRe[], double yIm[])
{
	int yIndx[maxPrimeFactor];
	int groupNo, blockNo, adr;
	double  yr, yi, twr, twi;

	double are0, aim0;
	double aRe[maxPrimeFactor], aIm[maxPrimeFactor];
	double vRe[maxPrimeFactorDiv2], vIm[maxPrimeFactorDiv2];
	double wRe[maxPrimeFactorDiv2], wIm[maxPrimeFactorDiv2];

    double  rere, reim, imre, imim;
    int     i,j,k,n,max;

	adr = groupOffset;
	for (groupNo=0; groupNo<remainRadix; groupNo++) {

		for (blockNo=0; blockNo<radix; blockNo++) {
			yIndx[blockNo] = adr;
			adr = adr + sofarRadix;
		}

		aRe[0] = yRe[yIndx[0]];
		aIm[0] = yIm[yIndx[0]];

		for (blockNo=1; blockNo<radix; blockNo += 1) {
			yr = yRe[yIndx[blockNo]]; yi = yIm[yIndx[blockNo]];
			twr = twiddleRe[blockNo]; twi = twiddleIm[blockNo];
			aRe[blockNo] =  twr * yr - twi * yi;
			aIm[blockNo] =  twr * yi + twi * yr;
		}

		are0 = aRe[0];
		aim0 = aIm[0];

		n = radix;
		max = (n + 1)/2;
		for (j=1; j < max; j++)	{
		  vRe[j] = aRe[j] + aRe[n-j];
		  vIm[j] = aIm[j] - aIm[n-j];
		  wRe[j] = aRe[j] - aRe[n-j];
		  wIm[j] = aIm[j] + aIm[n-j];
		}

		for (j=1; j < max; j++) {
			aRe[j]=are0; 
			aIm[j]=aim0;
			aRe[n-j]=are0; 
			aIm[n-j]=aim0;
			k=j;
			for (i=1; i < max; i++) {
				rere = trigRe[k] * vRe[i];
				imim = trigIm[k] * vIm[i];
				reim = trigRe[k] * wIm[i];
				imre = trigIm[k] * wRe[i];
            
				aRe[n-j] += rere + imim;
				aIm[n-j] += reim - imre;
				aRe[j]   += rere - imim;
				aIm[j]   += reim + imre;

				k = k + j;
				if (k >= n)  k = k - n;
			}
		}
		for (j=1; j < max; j++)	{
			aRe[0] = aRe[0] + vRe[j]; 
			aIm[0] = aIm[0] + wIm[j];
		}
		
		for (j=0; j < radix; j++) {
			yRe[yIndx[j]] = aRe[j]; 
			yIm[yIndx[j]] = aIm[j];
		}

		groupOffset=groupOffset+sofarRadix*radix;
		adr=groupOffset;
	}
}
	
void FFT_odd(int sofarRadix, int radix, int remainRadix, int groupOffset,
				  double trigRe[], double trigIm[],
				  double yRe[], double yIm[])
{
	int yIndx[maxPrimeFactor];
	int groupNo, blockNo, adr;

	double are0, aim0;
	double aRe[maxPrimeFactor], aIm[maxPrimeFactor];
	double vRe[maxPrimeFactorDiv2], vIm[maxPrimeFactorDiv2];
	double wRe[maxPrimeFactorDiv2], wIm[maxPrimeFactorDiv2];

    double  rere, reim, imre, imim;
    int     i,j,k,n,max;

	adr = groupOffset;
	for (groupNo=0; groupNo<remainRadix; groupNo++) {

		for (blockNo=0; blockNo<radix; blockNo++) {
			yIndx[blockNo] = adr;
			adr = adr + sofarRadix;
		}

		aRe[0] = yRe[yIndx[0]];
		aIm[0] = yIm[yIndx[0]];

		for (blockNo=1; blockNo<radix; blockNo += 1) {
			aRe[blockNo] =  yRe[yIndx[blockNo]];
			aIm[blockNo] =  yIm[yIndx[blockNo]];
		}

		are0 = aRe[0];
		aim0 = aIm[0];

		n = radix;
		max = (n + 1)/2;
		for (j=1; j < max; j++)	{
		  vRe[j] = aRe[j] + aRe[n-j];
		  vIm[j] = aIm[j] - aIm[n-j];
		  wRe[j] = aRe[j] - aRe[n-j];
		  wIm[j] = aIm[j] + aIm[n-j];
		}

		for (j=1; j < max; j++) {
			aRe[j]=are0; 
			aIm[j]=aim0;
			aRe[n-j]=are0; 
			aIm[n-j]=aim0;
			k=j;
			for (i=1; i < max; i++) {
				rere = trigRe[k] * vRe[i];
				imim = trigIm[k] * vIm[i];
				reim = trigRe[k] * wIm[i];
				imre = trigIm[k] * wRe[i];
            
				aRe[n-j] += rere + imim;
				aIm[n-j] += reim - imre;
				aRe[j]   += rere - imim;
				aIm[j]   += reim + imre;

				k = k + j;
				if (k >= n)  k = k - n;
			}
		}
		for (j=1; j < max; j++)	{
			aRe[0] = aRe[0] + vRe[j]; 
			aIm[0] = aIm[0] + wIm[j];
		}
		
		for (j=0; j < radix; j++) {
			yRe[yIndx[j]] = aRe[j]; 
			yIm[yIndx[j]] = aIm[j];
		}

		groupOffset=groupOffset+sofarRadix*radix;
		adr=groupOffset;
	}
}
	
