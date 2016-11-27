#include <stdlib.h>
#include <stdio.h>
#include "general.h"

double* multimtrx (const int dimension, const double* a, const double* b)
{
	if (dimension <= 0)
	{
		printf ("Incorrect dimension");
		return NULL;
	}
	double* result = (double*) malloc (dimension * dimension * sizeof(double));
	int i, j, k;
	for (i = 0; i < dimension; i++)
		for (j = 0; j < dimension; j++)
		{
			result[i * dimension + j] = 0;
			for (k = 0; k < dimension; k++)
				result[i * dimension + j] += a[i * dimension + k] * b [k * dimension + j];
		}
	return result;
}

double mabs (double x)
{
        return (x < 0) ? -x : x;
}

permut* Pcreate (const double* A, const int dimension)
{
	if (dimension < 0)
	{
		printf ("Incorrect dimension\n");
		return NULL;
	}
	permut* Pinfo = (permut*) malloc (sizeof (permut));
        Pinfo->P = (double*) malloc (dimension * dimension * sizeof (double));
        int i, j, k;
        for (i = 0; i < dimension; i++)
                for (j = 0; j < dimension; j++)
                        Pinfo->P[i * dimension + j] = (i == j); 
        for (i = 0; i < dimension; i++)
        {
                int maxj = i;
                for (j = i; j < dimension; j++)
                        if (abs(A[j * dimension + i]) > abs(A[maxj * dimension + i]))
                                maxj = j;
                if (maxj != i)
                        for (k = 0; k < dimension; k++)
                        {
                                double tmp = Pinfo->P[i * dimension + k]; 
                                Pinfo->P[i * dimension + k] = Pinfo->P[maxj * dimension + k]; 
                                Pinfo->P[maxj * dimension + k] = tmp;
                        }
		Pinfo->permut_number = k;
        }   
        return Pinfo;
}

double** LUcreate (const double* A, const double* P, const int dimension)
{
	if (dimension < 0)
	{
		printf ("Incorrect dimension\n");
		return NULL;
	}
	double* L = (double*) malloc (dimension * dimension * (sizeof (double)));
	double* U = (double*) malloc (dimension * dimension * (sizeof (double)));
	double* Apiv = multimtrx (dimension, P, A);
	int i, j, k;
	for (i = 0; i < dimension; i++)
	{
		L[i * dimension + i] = 1;
		for (j = 0; j < dimension; j++)
			U[i * dimension + j] = 0;
	}
	double tmp = 0;
	for (i = 0; i < dimension; i++)
	{
		for (j = 0; j < dimension; j++)
		{
			if (j <= i)
			{
				tmp = 0;
				for (k = 0; k < j; k++)
					tmp += L[j * dimension + k] * U[k * dimension + i];
				U[j * dimension + i] = Apiv[j * dimension + i] - tmp;
			}
			if (j >= i)
			{
				tmp = 0;
				for (k = 0; k < i; k++)
					tmp+= L[j * dimension + k] * U[k * dimension + i];
				L[j * dimension + i] = (Apiv[j * dimension + i] - tmp)/U[i * dimension + i];
			}
		}
	}
	free (Apiv);
	double** result = (double**) malloc (2 * sizeof (double*));
	result[0] = L;
	result[1] = U;
	return result;
}
