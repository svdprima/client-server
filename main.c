#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "general.h"

int main ()
{
	int n = 4;
	double* a = (double*) malloc (n * n * sizeof(double));
	double* b = (double*) malloc (n * n * sizeof(double));
	int i, j;
	srand (time (NULL));
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			a[i * n + j] = (double) (rand()%10); 
			b[i * n + j] = (double) (rand()%10);
		}
	int core_number = (int) sysconf(_SC_NPROCESSORS_ONLN);
	double* ab = multimtrx(n, a, b, core_number);
	printf ("A\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			printf ("%lf ", a[i * n + j]);
		printf ("\n");
	}
	printf ("B\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			printf ("%lf ", b[i * n + j]);
		printf ("\n");
	}	
	printf ("A * B\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			printf ("%lf ", ab[i * n + j]);
		printf ("\n");
	}	
	permut* prmt = Pcreate (ab, n);
	printf ("P\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			printf ("%lf ", prmt->P[i * n + j]);
		printf ("\n");
	}	
	double** ptr = LUcreate (ab, prmt->P, n, core_number);
	printf ("L\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			printf ("%lf ", ptr[0][i * n + j]);
		printf ("\n");
	}	
	printf ("U\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			printf ("%lf ", ptr[1][i * n + j]);
		printf ("\n");
	}	
	int determ = 1;
	for (i = 0; i < n; i++)
		determ *= ptr[1][i * n + i];
	int Pdet;
	Pdet = ((prmt->permut_number % 2) == 1) ? -1 : 1;
	determ *= Pdet;
	printf ("det (A*B) = %d\n", determ);
	free (a);
	free (b);
	free (prmt->P);
	free (prmt);
	for (i = 0; i < 2; i++)
		free (ptr[i]);
	free (ptr);	
	return 0;
}
