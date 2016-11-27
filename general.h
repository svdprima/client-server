struct permut
{
	double* P;
	unsigned int permut_number;
};
typedef struct permut permut;

double* multimtrx (const int dimension, const double* a, const double* b);
permut* Pcreate (const double* A, const int dimension);
double** LUcreate (const double* A, const double* P, const int dimension);
