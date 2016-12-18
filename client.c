#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include "general.h"

#define CHECK if (flag == -1) {printf ("%s\n", strerror (errno)); return -1;}

int main ()
{
	int input_fd = open ("client_input.txt", O_RDONLY);
	int flag = input_fd;
	CHECK
	const unsigned int b_size = lseek (input_fd, 0, SEEK_END);
	char buff [b_size];
	flag = lseek (input_fd, 0, SEEK_SET);
	CHECK 
	flag = read (input_fd, buff, b_size);
	CHECK
	unsigned int offset = 0;
	unsigned int n = get_number (buff, b_size, &offset);
	if (n <= 0)
	{
		printf ("Incorrect dimension\n");
		return -1;
	}
	key_t token = ftok ("client.txt", 1964);
	flag = token;
	CHECK
	int msid = msgget (token, S_IRWXU);
	flag = msid;
	CHECK
	mbuff* box = (mbuff*) malloc (sizeof(mbuff));
	box->mtype = 1;	
	box->data = n;
	state* lckd = (state*) malloc (sizeof(state));
	lckd->mtype = 2;
	lckd->is_locked = 1;
	flag = msgsnd (msid, box, sizeof(mbuff), 0);
	CHECK
	flag = msgrcv (msid, box, sizeof(mbuff), 0, 0);
	CHECK
	if (box->data == -2)
	{
		printf ("Incompatible dimensions\n Quiting...\n");
		return -1;
	}
	if (box->data != -1)
	{
		printf ("IPC protocol error\n");
		return -1;
	}
	flag = msgrcv (msid, box, sizeof(mbuff), 0, 0);
	CHECK
	const int shmid = box->data;
	double* mtrxs = (double*) shmat (shmid, NULL, 0);
	unsigned int i;
	for (i = 0; i < n * n; i++)
		mtrxs[i] = get_number (buff, b_size, &offset);
	printf ("Matrix loaded\n");
	lckd->is_locked = 0;
	flag = msgsnd (msid, lckd, sizeof(state), 0);
	CHECK
	printf ("Message sent\n");
	
	detcarry* det = (detcarry*) malloc (sizeof (detcarry));
	flag = msgrcv (msid, det, sizeof(detcarry), 3, 0);
	CHECK
	unsigned int j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			printf ("%lf ", mtrxs[n * (n + i) + j]);
		printf ("\n");
	}
	printf ("The determinant is %lld\n", det->deter);
	CHECK
	free (det);
	free (lckd);
	flag = shmctl (shmid, IPC_RMID, NULL);
        CHECK
	return 0;
}
