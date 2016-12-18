all:
	gcc server.c math.c io.c -Wall -Wextra -pthread -o server
	gcc client.c math.c io.c -Wall -Wextra -pthread -o client
