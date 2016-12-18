all:
	gcc server.c math.c io.c -Wall -Wextra -pthread -g -o server
	gcc client.c math.c io.c -Wall -Wextra -pthread -g -o client
