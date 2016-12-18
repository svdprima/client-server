#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "general.h"

void* pthr_multi (void* input)
{
	multi_parallel* info = (multi_parallel*) input;
	int i, j, k;
	int n = info->thread_num * info->rounds + info->last_steps;
	//printf ("Thread # %d has started its job\n", info->pthid);
	for (i = info->pthid; i < n; i += info->thread_num)
	{		
		for (j = 0; j < n; j ++)
		{
			info->answer[i * n + j] = 0;
			for (k = 0; k < n; k++)
				if (i < n)
					info->answer[i * n + j] += info->A[i * n + k] * info->B[k * n + j];
		}
	}
	//printf ("Thread # %d has finished its job\n", info->pthid);
	return NULL;			
}

void multimtrx (const int dimension, const double* a, const double* b, double* result, const int core_nu)
{
	if (dimension <= 0)
	{
		printf ("Incorrect dimension\n");
		return;
	}
	if (core_nu <= 0)
	{
		printf ("Incorrect core number\n");
		return;
	}
	int i;
	multi_parallel* data = (multi_parallel*) malloc (core_nu * sizeof (multi_parallel));
	int thread_nu;
	thread_nu = (dimension >= core_nu) ? core_nu : dimension;
	for (i = 0; i < thread_nu; i++)
	{
		(data + i)->answer = result;
		(data + i)->A = a;
		(data + i)->B = b;
		(data + i)->rounds = dimension/thread_nu;
		(data + i)->last_steps = dimension % (thread_nu * data->rounds);
		(data + i)->pthid = i;
		(data + i)->thread_num = thread_nu;
	}
	pthread_t* thr = (pthread_t*) malloc (thread_nu* sizeof(pthread_t));
	for (i = 0; i < thread_nu; i++)
	{	
		errno = pthread_create (thr + i, NULL, pthr_multi, (void*)(data + i));
		if (errno)
		{	
			printf ("%s\n", strerror (errno));
			return;
		}
	}
	for (i = 0; i < thread_nu; i++)
	{
		errno = pthread_join (thr[i], NULL);
		if (errno)
		{
			printf ("%s\n", strerror (errno));
			return;
		}
	}
	free (thr);
	free (data);
	return;
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

long long int determinant (const double* A, const permut* ptr, const int dimension, const int core_nu)
{
	if (dimension < 0)
	{
		printf ("Incorrect dimension\n");
		return -1;
	}
	double* L = (double*) malloc (dimension * dimension * (sizeof (double)));
	double* U = (double*) malloc (dimension * dimension * (sizeof (double)));
	double* Apiv = (double*) malloc (dimension * dimension * sizeof (double));
	multimtrx (dimension, ptr->P, A, Apiv, core_nu);
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
	long long answer = 1;
	for (i = 0; i < dimension; i++)	
		answer *= U[i* dimension + i];
	free (Apiv);
	free (L);
	free (U);
	return (ptr->permut_number % 2) ? -answer : answer;
}
