struct permut
{
	double* P;
	unsigned int permut_number;
};
typedef struct permut permut;

struct multi_parallel
{
	const double* A;
	const double* B;
	double* answer;
	unsigned int thread_num;
	unsigned int rounds;
	unsigned int last_steps;
	unsigned int row;
	unsigned short int pthid;
};
typedef struct multi_parallel multi_parallel;

double* multimtrx (const int dimension, const double* a, const double* b, const int core_nu);
permut* Pcreate (const double* A, const int dimension);
double** LUcreate (const double* A, const double* P, const int dimension, const int core_nu);
