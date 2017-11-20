#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#include "directory.h"
#include "poll.h"

#define BUFF_SIZE 2048
#define POLLS 2				/* STDIN, client_sock */

/* Check arguments is valid or not. If valid ip -> *serv_ip, port -> &serv_port */
void validArguments (int argc, char *argv[], char *serv_ip, int *serv_port);

/* Check wanna exit */
_Bool wannaExit (char *buff);

int main(int argc, char *argv[]) {
	int server_port = 0;
	char server_ip[16] = "";		// 16 = (max length of ipv4 string) + 1
	
	validArguments(argc, argv, server_ip, &server_port);

	int client_sock;
	char buff[BUFF_SIZE];
	struct sockaddr_in server_addr; /* server's address information */
	int msg_len, bytes_sent, bytes_received;

	client_sock = socket(AF_INET, SOCK_STREAM, 0);


	/* Init poll stdin, stdout, client_sock */
	struct pollfd polls[POLLS];
	polls[0].fd = 0; polls[0].events = POLLIN;				// STDIN
	polls[1].fd = client_sock; polls[0].events = POLLIN;	// client_sock

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = inet_addr(server_ip);

	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0) {
		printf("\nError!Can not connect to sever! Client exit imediately!\n");
		return 0;
	}

	// __fpurge(stdin);
	// __fpurge(stdout);
	system("clear");
	printf("Enter a command:\n");

	int revents;
	while(1){

		revents = poll(polls, POLLS, 100000);
		if (revents > 0) {
			if (polls[0].revents & POLLIN) {
				// __fpurge(stdin);
				system("clear");
				printf("Enter a command: ");

				bzero(buff, BUFF_SIZE);
				fgets(buff, BUFF_SIZE, stdin);
				buff[strlen(buff) - 1] = '\0';

				if (wannaExit(buff)) {
					printf("\n");
					return 0;
				}

				msg_len = strlen(buff) + 1;
				bytes_sent = send(client_sock, buff, msg_len, 0);
				if(bytes_sent <= 0){
					printf("\nConnection closed!\n");
					break;
				}
			}

			if (polls[1].revents & POLLIN) {
				system("clear");

				/* receive echo reply */
				bytes_received = recv(client_sock, buff, BUFF_SIZE-1, 0);
				if(bytes_received <= 0){
					printf("\nError!Cannot receive data from sever!\n");
					break;
				}

				buff[bytes_received] = '\0';
				printf("Reply from server: %s\n", buff);

				// Process 
				if (existFile(".", buff)) {
					printf("FOUND\n");
					// bytes_sent = send(client_sock, buff, bytes_received, 0);
					// if(bytes_received <= 0){
					// 	printf("\nError!Cannot receive data from sever!\n");
					// 	break;
					// }
				} else {
					printf("NOT FOUND\n");
					// bytes_sent = send(client_sock, "NOT FOUND", 10, 0);
					// if(bytes_received <= 0){
					// 	printf("\nError!Cannot receive data from sever!\n");
					// 	break;
					// }
				}
			}
		}
	}

	close(client_sock);
	return 0;
}

_Bool wannaExit (char *buff) {
	if (buff[0] == '\0' || buff[0] == '\n') return 1;
	else return 0;
}

void validArguments (int argc, char *argv[], char *serv_ip, int *serv_port) {
	if (argc > 2) {
		/* Check valid ip address */
		struct sockaddr_in tmp_addr;
		if (inet_pton(AF_INET, argv[1], &(tmp_addr.sin_addr)) == 0) {
			printf("IP Address is invalid\n!");
			exit(EXIT_FAILURE);
		} else {
			strcpy(serv_ip, argv[1]);
		}

		/* Check valid port */
		int i;
		char *port_str = argv[2];
		for (i = 0; port_str[i] != '\0'; i++) {
			if (!isdigit(port_str[i])) {
				printf("Port is invalid\n!");
				exit(EXIT_FAILURE);
			}
		}
		if (port_str[i] == '\0') *serv_port = atoi(port_str);
	} else {
		printf("(ERROR) To few arguments!\n");
		exit(EXIT_FAILURE);
	}
}
