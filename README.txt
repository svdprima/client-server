Client and server communicate through the message queue. The token "seed" 1964 is hardcoded to secure access to the same mqueue.

Both client and server take data from txt files 'client_input.txt' and 'server_input.txt'. The first number in the files is the dimension of the matrix, then the lines of the matrix follow. Server is in an infinte circle. Client sends the dimension of its matrix, and if it is not equal to the server's one, client exits. If the dimensions are all right, server passes to the client the shmid. Client gets the piece of shared memory, loads its matrix there and then sends a signal to server. Server multiplicates the matrixes, counts the determinant, places the matrix into the shared memory and sends the determinant to the client via mqueue. The client displays the data.

The determinant is computed using LU decomposition. Before that, a permutation matrix is calculated to reduce the error in double-type ariphmetics. Multiplication is paralleled with POSIX threads, so it will not run on Windows. System function sysconf() is used to get the number of active cores. The number of threads is min (core_number, dimension).

It is strongly advised to quit from server using "CTRL \" key combination. It sends signal #3 and server calls all nessesary destructors to clean up allocated memory and mqueue.
