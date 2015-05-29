
/************************************************************************
  fftbench
 ------------------------------------------------------------------------
  Function:
      The FFT routine under test is characterized in terms of speed and
      accuracy. The routine is tested at different FFT lengths and the
      test results are saved in the file FFTBENCH.TXT.
      
      The FFT benchmark program is distributed with the mixed radix FFT
      routine. If you want to test another FFT routine, you probably only
      have to modify the make file, the FFT routine declaration and the 
      fftio routine below.
 ------------------------------------------------------------------------
  Author:
      Jens Joergen Nielsen            For non-commercial use only.
      Kloevervang 7                   A $100 fee must be paid if used
      DK-2970 Hoersholm               commercially. Please contact.
      DENMARK

      E-mail : jjn2970@gmail.com      All rights reserved. October 2000-2015.
      Homepage : http://home.corix.dk/html/fft.html
*************************************************************************/

#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <sys\timeb.h>
#include "mixfft.h"

#define  maxIndex  20000000L


/* The purpose of this routine, which is called from the benchmark  
 * program, is to convert between the data formats used in the      
 * benchmark routine and the formats used in the FFT routine under  
 * test.                                                            
 */
int fftio(long n,double are[],double aim[],double bre[],double bim[])
{
    fft((int)n, are, aim, bre, bim);
    return(0);
}

int fftInvIo(long n,double are[],double aim[],double bre[],double bim[])
{
    fftInv((int)n, are, aim, bre, bim);
    return(0);
}

/* Calculate time in seconds from start. Resolution 1/1000 second.
 */
double timeUsed(struct _timeb *start)
{                               
    double seconds;
    struct _timeb   finish;

    _ftime_s(&finish);
    seconds = (double) finish.millitm;
    seconds = seconds - start->millitm;
    seconds = seconds / 1000;
    seconds = seconds + finish.time - start->time;
    
    return(seconds);
}


/* Measure the time taken to perform an FFT. The test signal is a 
 * linear sweep from 0 to fs/2. At least 2 seconds of test time is
 * used and output is the average time for one FFT in seconds.
 */
double fftTime(long fftLength, long minTime)
{
    long i, k, n, m, q;  
    double w, pi;
    double elapsed_time;
    struct _timeb   start;

    double   *xre, *xim, *yre, *yim; 
	
    xre = (double  *) calloc(fftLength, sizeof(double));                        
    xim = (double  *) calloc(fftLength, sizeof(double));                        
    yre = (double  *) calloc(fftLength, sizeof(double));
    yim = (double  *) calloc(fftLength, sizeof(double));
                  
    n = fftLength;                             
    pi = 4 * atan(1); 
    k  = 1;
    w  = k*pi/n/n;
    for (i=0; i<n; i++) {
        xre[i] = cos(i*i*w);
        xim[i] = sin(i*i*w);
    }                    
    
    m = 0;
    q = 1;            
    _ftime_s(&start);
    while (timeUsed(&start) < minTime)
    {
        for (i=0; i<q; i++)
            fftio(n, xre, xim, yre, yim);
        m = m + q;
        q = q + q;
    }
    elapsed_time = timeUsed(&start)/(double)m;

    free(yim);
    free(yre);
    free(xim);
    free(xre);
    
	return(elapsed_time);
}

double fftTimeIQ(long fftLength, long minTime)
{
    long i, k, n, m, q;  
    double w, pi;
    double elapsed_time;
    struct _timeb   start;
	struct fftContext fc;

    double   *xIQ, *yre, *yim; 
	
    xIQ = (double  *) calloc(2*fftLength, sizeof(double));                        
    yre = (double  *) calloc(fftLength, sizeof(double));
    yim = (double  *) calloc(fftLength, sizeof(double));

	createFftContext(fftLength, &fc);

                  
    n = fftLength;                             
    pi = 4 * atan(1); 
    k  = 1;
    w  = k*pi/n/n;
    for (i=0; i<n; i++) {
        xIQ[2*i] = cos(i*i*w);
        xIQ[2*i+1] = sin(i*i*w);
    }                    
    
    m = 0;
    q = 1;            
    _ftime_s(&start);
    while (timeUsed(&start) < minTime)
    {
        for (i=0; i<q; i++)
			fftCtxtIQ(&fc, xIQ, yre, yim);
        m = m + q;
        q = q + q;
    }
    elapsed_time = timeUsed(&start)/(double)m;

	freeFftContext(&fc);
    free(yim);
    free(yre);
    free(xIQ);
    
	return(elapsed_time);
}

/* Measure the difference between the theoretical FFT output and the
 * actual FFT output. The power leak is measured in terms of error 
 * to signal power ratio. The value is returned in dB.
 */
double fftLeak(long fftLength)
{
    long i, k, n;  
    double w, pi;
    double sqr, sqrsum, sqrmax;
    double *xre, *xim, *yre, *yim; 
	
    xre = (double  *) calloc(fftLength, sizeof(double));                        
    xim = (double  *) calloc(fftLength, sizeof(double));                        
    yre = (double  *) calloc(fftLength, sizeof(double));
    yim = (double  *) calloc(fftLength, sizeof(double));

    n = fftLength;                             
    pi =  3.141592653589793238462643383; 
    k = 3;
    w = 2*k*pi/n;
    for (i=0; i<n; i++) {
        xre[i] = cos(i*w);
        xim[i] = sin(i*w);
    }                    
                     
    fftio(n, xre, xim, yre, yim);
//    fftInvIo(n, yre, yim, xre, xim); //to test the accuracy of 2xfft + inverse fft
//    fftio(n, xre, xim, yre, yim);    //enable this line too

    sqrsum = (yre[k]-n)*(yre[k]-n) + yim[k]*yim[k];
    sqrmax = 0;         
    for (i=0; i<n; i++)
        if (i != k)
        {
            sqr = yre[i]*yre[i] + yim[i]*yim[i];
            sqrsum = sqrsum + sqr;
            if (sqr > sqrmax) sqrmax = sqr;
        }
    sqrsum = sqrsum/n;    /* division by n*n in two stages to avoid */
    sqrsum = sqrsum/n;    /* integer overflow !!!!                  */
    sqrmax = sqrmax/n;
    sqrmax = sqrmax/n;
             
    if (sqrsum > 0)
        sqrsum = 10*log10(sqrsum);
    else                                
        sqrsum = -1000;
    if (sqrmax > 0)
        sqrmax = 10*log10(sqrmax);
    else
        sqrmax = -1000;
        
    free(yim);
    free(yre);
    free(xim);
    free(xre);
    return(sqrsum);
}

double fftLeakIQ(long fftLength)
{
    long i, k, n;  
    double w, pi;
    double sqr, sqrsum, sqrmax;
	struct fftContext fc;
    double *xIQ, *yre, *yim; 
	
    xIQ = (double  *) calloc(2*fftLength, sizeof(double));                        
    yre = (double  *) calloc(fftLength, sizeof(double));
    yim = (double  *) calloc(fftLength, sizeof(double));

	createFftContext(fftLength, &fc);

    n = fftLength;                             
    pi =  3.141592653589793238462643383; 
    k = 3;
    w = 2*k*pi/n;
    for (i=0; i<n; i++) {
        xIQ[2*i] = cos(i*w);
        xIQ[2*i+1] = sin(i*w);
    }                    
                     
//    fftio(n, xre, xim, yre, yim);
	fftCtxtIQ(&fc, xIQ, yre, yim);

    sqrsum = (yre[k]-n)*(yre[k]-n) + yim[k]*yim[k];
    sqrmax = 0;         
    for (i=0; i<n; i++)
        if (i != k)
        {
            sqr = yre[i]*yre[i] + yim[i]*yim[i];
            sqrsum = sqrsum + sqr;
            if (sqr > sqrmax) sqrmax = sqr;
        }
    sqrsum = sqrsum/n;    /* division by n*n in two stages to avoid */
    sqrsum = sqrsum/n;    /* integer overflow !!!!                  */
    sqrmax = sqrmax/n;
    sqrmax = sqrmax/n;
             
    if (sqrsum > 0)
        sqrsum = 10*log10(sqrsum);
    else                                
        sqrsum = -1000;
    if (sqrmax > 0)
        sqrmax = 10*log10(sqrmax);
    else
        sqrmax = -1000;
        
	freeFftContext(&fc);
    free(yim);
    free(yre);
    free(xIQ);
    return(sqrsum);
}

/* Measure the difference between the theoretical inverse FFT output and the
 * actual inverse FFT output. The power leak is measured in terms of error 
 * to signal power ratio. The value is returned in dB.
 */
double fftInvLeak(long fftLength)
{
    long     i, k, n;  
    double   w, pi;
    double   sqr, sqrsum, sqrmax;
    double   *xre, *xim, *yre, *yim; 
	
    xre = (double  *) calloc(fftLength, sizeof(double));                        
    xim = (double  *) calloc(fftLength, sizeof(double));                        
    yre = (double  *) calloc(fftLength, sizeof(double));
    yim = (double  *) calloc(fftLength, sizeof(double));

    n = fftLength;                             
    pi =  3.141592653589793238462643383; 
    k = 3;
    w = 2*k*pi/n;
    for (i=0; i<n; i++) {
        xre[i] = cos(i*w);
        xim[i] = -sin(i*w);
    }                    
                     
    fftInvIo(n, xre, xim, yre, yim);

    sqrsum = (yre[k]-1)*(yre[k]-1) + yim[k]*yim[k];
    sqrmax = 0;         
    for (i=0; i<n; i++)
        if (i != k)
        {
            sqr = yre[i]*yre[i] + yim[i]*yim[i];
            sqrsum = sqrsum + sqr;
            if (sqr > sqrmax) sqrmax = sqr;
        }

             
    if (sqrsum > 0)
        sqrsum = 10*log10(sqrsum);
    else                                
        sqrsum = -1000;
    if (sqrmax > 0)
        sqrmax = 10*log10(sqrmax);
    else
        sqrmax = -1000;
        
    free(yim);
    free(yre);
    free(xim);
    free(xre);
    return(sqrsum);
}


/* FFT benchmark main program.
 */
void main()
{                
    double t1, p1, p1inv;
    double t2, p2;
    long i, n;
    long nmax, nfft[maxPrimeFactor];
	char buffer[200];
	int j;
    FILE *txtFile;

    printf("\nFFT Benchmark Test");        
    printf("\n\nMaximum FFT Length   : %10d", maxIndex);        
    printf("\nMaximum Prime Factor : %10d", maxPrimeFactor);        
    fopen_s(&txtFile, "fftbench.txt", "w");
    fprintf(txtFile,"\nResults from FFT benchmark test:");
              
    do {        
        printf("\n\n");
        printf("(0)  End FFT benchmark test.\n");
        printf("(1)  FFT lengths : 128, 256, 512, ... , 8192.\n");
        printf("(2)  FFT lengths : 100, 200, 500, ... , 10000.\n");
        printf("(3)  FFT lengths : 4, 5, 6, 7, ... , maxPrimeFactor.\n");
        printf("(4)  FFT lengths : 4, 8, 16, 32, ... , 2^N <= maxIndex.\n");
        printf("(5)  FFT lengths : 10, 100, 1000, ... , 10^N <= maxIndex.\n");
        printf("(6)  FFT lengths : 72 88 112 128 176 224 256 288 352 512 576.\n");
        printf("(9)  User selectable FFT length.\n\n");
        printf("Enter selection, 0-4, 9 : ");
        scanf_s( "%ld", &n );
        nmax = 0;
        switch(n) {
            case 0 : nmax = 0; break;
            case 1 : 
                nmax = 7;
                nfft[0]  =  128;
                nfft[1]  =  256;
                nfft[2]  =  512;
                nfft[3]  = 1024;
                nfft[4]  = 2048;
                nfft[5]  = 4096;
                nfft[6]  = 8192;
                break;
            case 2 : 
                nmax = 7;
                nfft[0]  =   100;
                nfft[1]  =   200;
                nfft[2]  =   500;
                nfft[3]  =  1000;
                nfft[4]  =  2000;
                nfft[5]  =  5000;
                nfft[6]  = 10000;
                break;
            case 3 : 
                nmax = maxPrimeFactor-3;
				for (i=4; i<=maxPrimeFactor; i++) {
					nfft[i-4]  =   i;
				}
                break;
            case 4 : 
				nmax = 0;

				for (i=4; i<=maxIndex; i = i + i) {
					nfft[nmax]  =   i;
					nmax++;
				}
                break;
            case 5 : 
				nmax = 0;

				for (i=10; i<=maxIndex; i = 10*i) {
					nfft[nmax]  =   i;
					nmax++;
				}
                break;
            case 6 : 
				nmax = 0;

				nfft[nmax] =  72; nmax++;
				nfft[nmax] =  88; nmax++;
				nfft[nmax] = 112; nmax++;
				nfft[nmax] = 128; nmax++;
				nfft[nmax] = 176; nmax++;
				nfft[nmax] = 224; nmax++;
				nfft[nmax] = 256; nmax++;
				nfft[nmax] = 288; nmax++;
				nfft[nmax] = 352; nmax++;
				nfft[nmax] = 512; nmax++;
				nfft[nmax] = 576; nmax++;
                break;

            case 9 : 
                printf("\nEnter FFT length, 4<=N<=%5ld, maxPrime <= %5ld     : ", 
					    maxIndex, maxPrimeFactor);
                scanf_s( "%ld", &n );
                if ((n > 3) && (n <= maxIndex)) {
                    nfft[0] = n;
                    nmax = 1;
                }
                break;
        }

        j = 0;
		j += sprintf_s( buffer+j, 200-j, "\n %8s ", "Length" ); 
		j += sprintf_s( buffer+j, 200-j, " %5s ms", "Simpl" ); 
		j += sprintf_s( buffer+j, 200-j, " %5s dB", "Leak" );
		j += sprintf_s( buffer+j, 200-j, " %5s dB", "Inv" );
		j += sprintf_s( buffer+j, 200-j, " %5s ms", "Table" ); 
		j += sprintf_s( buffer+j, 200-j, " %5s dB", "Leak" );
		j += sprintf_s( buffer+j, 200-j, " %7s", "Change%%" );
		printf(buffer);
		fprintf(txtFile,buffer);

		for (i=0; i<nmax; i++) {
            t1 = fftTime( nfft[i], 1);
            p1 = fftLeak( nfft[i]);
            p1inv = fftInvLeak( nfft[i]);
            t2 = fftTimeIQ( nfft[i], 1);
            p2 = fftLeakIQ( nfft[i]);
	        j = 0;
			j += sprintf_s( buffer+j, 200-j, "\n %8ld", nfft[i]); 
			j += sprintf_s( buffer+j, 200-j, " %8.4f", 1000*t1); 
			j += sprintf_s( buffer+j, 200-j, " %8.1f", p1);
			j += sprintf_s( buffer+j, 200-j, " %8.1f", p1inv);
			j += sprintf_s( buffer+j, 200-j, " %8.4f", 1000*t2); 
			j += sprintf_s( buffer+j, 200-j, " %8.1f", p2);
			j += sprintf_s( buffer+j, 200-j, " %8.1f", 100*(t2-t1)/t1);
			printf(buffer);
			fprintf(txtFile,buffer);
        }                    
    } while(n > 0);

    fclose(txtFile);   

    printf("\n");
    printf("\nFFT test terminated.");
    printf("\nCopyright 2015, Jens Joergen Nielsen, jjn2970@gmail.com");
    printf("\n");
    
 }



