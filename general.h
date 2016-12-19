#pragma once
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

struct mbuff 
{
	long mtype;
	int data;
};
typedef struct mbuff mbuff;

struct state
{
	long mtype;
	int is_locked;
};
typedef struct state state;

struct init
{
	long mtype;
	int foo;
};
typedef struct init init;

struct detcarry
{
	long mtype;
	long long int deter;
};
typedef struct detcarry detcarry;

int get_number (char* inp_string, const unsigned int buff_size, unsigned int* offset);
void multimtrx (const int dimension, const double* a, const double* b, double* result, const int core_nu);
permut* Pcreate (const double* A, const int dimension);
long long int determinant (const double* A, const permut* ptr, const int dimension, const int core_nu);
