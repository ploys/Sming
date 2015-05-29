#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void twiddleFFT_2(int sofarRadix, int radix, int remainRadix, int groupOffset,
				  double twiddleRe[], double twiddleIm[],
				  double yRe[], double yIm[])
{
	int yIndx[2];
	int groupNo, blockNo, adr;
	double yr, yi, twr, twi;
	double  ar0, ar1;
	double  ai0, ai1;

	adr = groupOffset;
	for (groupNo=0; groupNo<remainRadix; groupNo++) {
		for (blockNo=0; blockNo<radix; blockNo++) {
			yIndx[blockNo] = adr;
			adr = adr + sofarRadix;
		}
		ar0 = yRe[yIndx[0]];
		ai0 = yIm[yIndx[0]];

		yr = yRe[yIndx[1]];
		yi = yIm[yIndx[1]];
		twr = twiddleRe[1];
		twi = twiddleIm[1];
		ar1=  twr * yr - twi * yi;
		ai1=  twr * yi + twi * yr;

		yRe[yIndx[0]] = ar0 + ar1;
		yRe[yIndx[1]] = ar0 - ar1; 
		yIm[yIndx[0]] = ai0 + ai1;
		yIm[yIndx[1]] = ai0 - ai1;

		groupOffset=groupOffset+sofarRadix*radix;
		adr=groupOffset;
	}
}
	
void FFT_2(int sofarRadix, int radix, int remainRadix, int groupOffset,
				  double yRe[], double yIm[])
{
	int yIndx[2];
	int groupNo, blockNo, adr;
	double  ar0, ar1;
	double  ai0, ai1;

	adr = groupOffset;
	for (groupNo=0; groupNo<remainRadix; groupNo++) {
		for (blockNo=0; blockNo<radix; blockNo++) {
			yIndx[blockNo] = adr;
			adr = adr + sofarRadix;
		}
		ar0 = yRe[yIndx[0]];
		ai0 = yIm[yIndx[0]];

		ar1 = yRe[yIndx[1]];
		ai1 = yIm[yIndx[1]];

		yRe[yIndx[0]] = ar0 + ar1;
		yRe[yIndx[1]] = ar0 - ar1; 
		yIm[yIndx[0]] = ai0 + ai1;
		yIm[yIndx[1]] = ai0 - ai1;

		groupOffset=groupOffset+sofarRadix*radix;
		adr=groupOffset;
	}
}
	
