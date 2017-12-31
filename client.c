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
#include <transent/command.h>
#include <transent/tsfmanage.h>
#define POLLS 2
#define DATA_PATH "./db"

/* Check arguments is valid or not. If valid ip -> *serv_ip, port -> &serv_port */
void validArguments (int argc, char *argv[], char *serv_ip, int *serv_port);
int local_interac(struct pollfd poll, char* buff, char* payload, int sockfd);
int server_interac(struct pollfd poll, char* buff, char* payload, int sockfd);
/* Check wanna exit */
_Bool wannaExit (char *buff);

int main(int argc, char *argv[]) {
	int 	server_port = 0;
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

	fprintf(stderr,"Enter command:");
	while(1){
		revents = poll(polls, POLLS, 20000);
		if (revents > 0) {
			if (polls[0].revents & POLLIN)
				local_interac(polls[0],buff,payload,client_sock);

			if (polls[1].revents & POLLIN)
				server_interac(polls[1],buff,payload,client_sock);
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


int local_interac(struct pollfd poll, char* buff, char* payload, int sockfd){
	Method method = STUP;
	Command* cmd = NULL;
	int	bytes_sent 	= 0,
		msg_len 	= 0;

	printf("Enter command:");
	bzero(payload,PAY_LEN);
	scanf("%[^\n]",payload);
	while(getchar()!='\n');
	cmd = parse_cmd(payload);
	method = valid_cmd(*cmd);

	if(method == UNDEFINE){
		exit(1);
	}
	
	// while(getchar()!='\n');
	// attach_payload(buff,payload,strlen(payload));
	// loginfo("Payload:%s|len:%d|req:%d\n",detach_payload(buff),
	// 									get_payload_size(buff),
	// 									extract_request(buff));
	// if (wannaExit(payload)){
	// 	printf("\n");
	// 	return 0;
	// }

	// msg_len = get_real_len(buff);
	// bytes_sent = send(sockfd, buff, msg_len, 0);
	// if(bytes_sent <= 0){
	// 	printf("\nConnection closed!\n");
	// 	return 0;
	// }
	
	// loginfo("Send:%dbyte\n",get_real_len(buff));
	return 1;
}

int server_interac(struct pollfd poll,char* buff, char* payload,int sockfd){
	// int req_response   = UNDEFINE,
	// 	msg_len 	   = 0, 
	// 	bytes_transfer = 0;

	// bzero(buff,BUFF_SIZE);
	// bytes_transfer = recv(sockfd, buff, BUFF_SIZE-1, 0);
	// if(bytes_transfer <= 0){
	// 	printf("\nError!Cannot receive data from sever!\n");
	// 	return 0;
	// }

	// req_response = parse_packet(buff,payload,&bytes_transfer);

	// if(req_response == RQ_FILE){
	// 	char* filename = detach_payload(buff);
	// 	loginfo("filename:%s\n",filename);
	// 	if(existFile(DATA_PATH,filename)){
	// 		add_request(buff,RP_FOUND);
	// 		attach_payload(buff,filename,0);
	// 		msg_len = get_real_len(buff);
	// 		msg_len = send(sockfd, buff, msg_len, 0);
	// 		loginfo("Founded!\n");
	// 	}else{
	// 		add_request(buff,RP_NFOUND);
	// 		msg_len = send(sockfd, buff, msg_len, 0);
	// 		loginfo("Not found:%s\n",buff+HEADER_LEN);
	// 	}
	// 	free(filename);
	// }else if(req_response == RP_FLIST){
		
	// }else if(req_response == NOTI_INF){
	// 	printf("Server noti:%s\n",payload);
	// }
	return 0;
}