#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <transent/directory.h>
#include <poll.h>
#include <unistd.h>

#define DEBUG 1
#include <transent/util.h>
#include <transent/interface.h>
#define POLLS 2

/* Check arguments is valid or not. If valid ip -> *serv_ip, port -> &serv_port */
void validArguments (int argc, char *argv[], char *serv_ip, int *serv_port);

/* Check wanna exit */
_Bool wannaExit (char *buff);

int main(int argc, char *argv[]) {
	int 	server_port = 0;
	int 	method = UNDEFINE;
	int 	revents = -1;
	int 	client_sock;
	int 	msg_len, 
			bytes_sent, 
			bytes_received;
	char 	server_ip[16] = "";
	char 	buff[BUFF_SIZE],
			payload[PAY_LEN];
	struct 	pollfd polls[POLLS];
	struct 	sockaddr_in server_addr;

	logwarn("Start program");
	validArguments(argc, argv, server_ip, &server_port);

	client_sock = socket(AF_INET, SOCK_STREAM, 0);

	/* Init poll stdin, stdout, client_sock */
	polls[0].fd = 0; 
	polls[0].events = POLLOUT|POLLRDNORM;
	polls[1].fd = client_sock;
	polls[1].events = POLLIN;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = inet_addr(server_ip);

	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0) {
		printf("\nError!Can not connect to sever! Client exit imediately!\n");
		return 0;
	}

	method = get_user_method(0);
	
	if(method==LOGOUT) 
		goto end_process;

	while(1){
		revents = poll(polls, POLLS, 20000);
		if (revents > 0) {
			if (polls[0].revents & (POLLOUT|POLLRDNORM)) {
				if(method==SENDFILE)
					printf("Enter file name:");
				
				add_request(buff,RQ_FILE);
				fgets(payload, PAY_LEN, stdin);
				attach_payload(buff,payload,strlen(payload));
				loginfo("Payload:%s\n",detach_payload(buff));
				
				if (wannaExit(payload)){
					printf("\n");
					return 0;
				}

				msg_len = get_real_len(buff);
				loginfo("Real len:%d\n",msg_len);

				bytes_sent = send(client_sock, buff, msg_len, 0);

				if(bytes_sent <= 0){
					printf("\nConnection closed!\n");
					break;
				}
			}
			 
			if (polls[1].revents & POLLIN) {
				/* receive echo reply */
				bytes_received = recv(client_sock, buff, BUFF_SIZE-1, 0);

				if(bytes_received <= 0){
					printf("\nError!Cannot receive data from sever!\n");
					break;
				}

				buff[bytes_received] = '\0';
				printf("Reply from server: %s\n", buff);

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
		}else if(revents == 0){
			printf("Timeout!");
		}
	}
	end_process:
	close(client_sock);
	return 0;
}

_Bool wannaExit(char *buff) {
	if (buff[0] == '\0' || buff[0] == '\n') return 1;
	else return 0;
}

void validArguments (int argc, char *argv[], char *serv_ip, int *serv_port) {
	if (argc > 2) {
		/* Check valid ip address */
		struct sockaddr_in tmp_addr;
		if (inet_pton(AF_INET, argv[1], &(tmp_addr.sin_addr)) == 0) {
			printf(ADDERR);
			exit(EXIT_FAILURE);
		} else {
			strcpy(serv_ip, argv[1]);
		}

		/* Check valid port */
		int i;
		char *port_str = argv[2];
		for (i = 0; port_str[i] != '\0'; i++) {
			if (!isdigit(port_str[i])) {
				printf(PARGERR);
				exit(EXIT_FAILURE);
			}
		}
		if (port_str[i] == '\0') *serv_port = atoi(port_str);
	} else {
		printf(ARGERR);
		exit(EXIT_FAILURE);
	}
}
