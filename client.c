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
#include <transent/tsfmanage.h>
#define POLLS 2
#define DATA_PATH "./db"

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
	polls[0].events = POLLIN;//OUT|POLLRDNORM;
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
	if(method==UNKNOWN) 
		goto end_process;
	if(method==LOGOUT) 
		goto end_process;

	while(1){
		revents = poll(polls, POLLS, 20000);
		if (revents > 0) {
			if (polls[0].revents & POLLIN){//OUT|POLLRDNORM)) {
				if(method==SENDFILE)
					fprintf(stderr,"Enter file name:");

				bzero(payload,PAY_LEN);
				add_request(buff,RQ_FILE);
				scanf("%[^\n]",payload);
				while(getchar()!='\n');
				attach_payload(buff,payload,strlen(payload));
				loginfo("Payload:%s|len:%d|req:%d\n",detach_payload(buff),get_payload_size(buff),extract_request(buff));
				if (wannaExit(payload)){
					printf("\n");
					return 0;
				}
				msg_len = get_real_len(buff);
				bytes_sent = send(client_sock, buff, msg_len, 0);
				if(bytes_sent <= 0){
					printf("\nConnection closed!\n");
					break;
				}
				loginfo("Send:%dbyte\n",get_real_len(buff));
			}
			
			if (polls[1].revents & POLLIN) {
				/* receive echo reply */
				int req_response = UNDEFINE;
				bzero(buff,BUFF_SIZE);
				bytes_received = recv(client_sock, buff, BUFF_SIZE-1, 0);
				if(bytes_received <= 0){
					printf("\nError!Cannot receive data from sever!\n");
					break;
				}

				req_response = parse_packet(buff,payload,&bytes_received);

				if(req_response == RQ_FILE){
					char* filename = detach_payload(buff);
					loginfo("filename:%s\n",filename);
					if(existFile(DATA_PATH,filename)){
						add_request(buff,RP_FOUND);
						attach_payload(buff,filename,0);
						msg_len = get_real_len(buff);
						msg_len = send(client_sock, buff, msg_len, 0);
						loginfo("Founded!\n");
					}else{
						add_request(buff,RP_NFOUND);
						msg_len = send(client_sock, buff, msg_len, 0);
						loginfo("Not found:%s\n",buff+HEADER_LEN);
					}
					free(filename);
				}else if(req_response == RP_FLIST){
					
				}else if(req_response == NOTI_INF){
					printf("Server noti:%s\n",payload);
				}
			}
		}else if(revents == 0){
			printf("Timeout!\n");
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
