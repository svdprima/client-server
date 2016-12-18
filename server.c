#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/shm.h>
#include <signal.h>
#include "general.h"

#define CHECK if (flag == -1) {printf ("%s\n", strerror (errno)); return -1;}

int shmid;
int cl_shmid;
mbuff* cont;
state* lock;
permut* pmtrx;
int msid;

void eraser (int foo)
{
	if (foo == 3)
	{
		msgctl (msid, IPC_RMID, NULL);
		shmctl (shmid, IPC_RMID, NULL);	
		shmctl (cl_shmid, IPC_RMID, NULL);
		free (cont);
		free (lock);
		free (pmtrx);
		printf ("Everything has been cleaned\n");
		exit (0);
	}
	else 
	{
		printf ("Incorrect abortion!\n");
		exit (1);
	}
}

int main ()
{
        int input_fd = open ("server_input.txt", O_RDONLY);
        int flag = input_fd;
        const unsigned int b_size = lseek (input_fd, 0, SEEK_END);
	char buff [b_size];
        flag = lseek (input_fd, 0, SEEK_SET);
        CHECK
        flag = read (input_fd, buff, b_size);
        CHECK
        unsigned int offset = 0;
        unsigned int n = get_number (buff, b_size, &offset);
	shmid = shmget (IPC_PRIVATE, n * n * sizeof(double), IPC_CREAT | S_IRWXU);
	flag = shmid;
	CHECK
	double* a = (double*) shmat (shmid, NULL, 0);
        CHECK
        if (n <= 0)
        {
                printf ("Incorrect dimension\n");
                return -1;
        }
	unsigned int i;
	for (i = 0; i < n * n; i++)
		a[i] = get_number (buff, b_size, &offset);
	pmtrx = Pcreate (a, n);
	cont = (mbuff*) malloc (sizeof(mbuff));
	cont->mtype = 1;
	cont->data = 1;
	int sec_fd = open ("client.txt", O_CREAT | O_RDWR, S_IRWXU);
	flag = sec_fd;
	CHECK
	key_t token = ftok("client.txt", 1964);
	flag = token;
	CHECK
	cl_shmid = shmget (token, (2 * n * n + 1) * sizeof (double), IPC_CREAT | S_IRWXU);
	flag = cl_shmid;
	CHECK
	double* arr = (double*) shmat (cl_shmid, NULL, 0);
	const unsigned int cores = sysconf (_SC_NPROCESSORS_ONLN);
	msid = msgget (token, IPC_CREAT | S_IRWXU);
	flag = msid;
	CHECK
	state* lock = (state*) malloc (sizeof(lock));
	lock->mtype = 2;
	lock->is_locked = 0;
	unsigned int client_count = 0;
	while (1)
	{
		signal (3, eraser);
		flag = msgrcv (msid, cont, sizeof (mbuff), 0, 0);
		CHECK
		cont->data = (cont->data != n) ? -2 : -1;
		flag = msgsnd (msid, cont, sizeof (mbuff), 0);
		CHECK
		cont->data = cl_shmid;
		flag = msgsnd (msid, cont, sizeof (mbuff), 0);
		CHECK	
		flag = msgrcv (msid, lock, sizeof (state), 2, 0);
		CHECK
		multimtrx (n, arr, a, arr + n * n, cores);
		detcarry* ptr = (detcarry*) malloc (sizeof (detcarry));	
		ptr->mtype = 3;
		ptr->deter = determinant (arr + n * n, pmtrx, n, cores);
		flag = msgsnd (msid, ptr, sizeof(detcarry), 0);
		CHECK
		client_count++;
		printf ("Client #%d disconnected\n", client_count);
	}
	return 0;
}
