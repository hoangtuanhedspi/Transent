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
int local_interac(char* buff, char* payload, int sockfd);
int server_interac(char* buff, char* payload, int sockfd);
/* Check wanna exit */
_Bool wannaExit (char *buff);

int main(int argc, char *argv[]) {
	int 	server_port = 0;
	int 	revents = -1;
	int 	client_sock;
	int 	res;
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
				res = local_interac(buff,payload,client_sock);

			if (polls[1].revents & POLLIN)
				res = server_interac(buff,payload,client_sock);
			if(res==0) break;
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


int local_interac(char* buff, char* payload, int sockfd){
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
		loginfo("METHOD:%d\n",method);
		exit(1);
	}

	if(method == FIND){
		add_request(buff,RQ_FILE);
		attach_payload(buff,cmd->data,strlen(cmd->data));
		msg_len = get_real_len(buff);
		bytes_sent = send(sockfd, buff, msg_len, 0);
	}

	if(bytes_sent <= 0){
		printf("\nConnection closed!\n");
		return 0;
	}

	return 1;
}

int server_interac(char* buff, char* payload,int sockfd){
	int req_response   = UNDEFINE,
		msg_len 	   = 0, 
		bytes_transfer = 0;

	bzero(buff,BUFF_SIZE);

	bytes_transfer = recv(sockfd, buff, BUFF_SIZE-1, 0);
	if(bytes_transfer <= 0){
		printf("\nError!Cannot receive data from sever!\n");
		return 0;
	}

	req_response = parse_packet(buff,payload,&bytes_transfer);
	printf("on_%s with response %d\n",__func__,req_response);
	if(req_response == RQ_FILE){
		char* filename = detach_payload(buff);
		if(existFile(DATA_PATH,filename)){
			add_request(buff,RP_FOUND);
			attach_payload(buff,filename,strlen(filename));
			msg_len = get_real_len(buff);
			msg_len = send(sockfd, buff, msg_len, 0);
			if(msg_len<0)
				return 0;
		}else{
			add_request(buff,RP_NFOUND);
			msg_len = send(sockfd, buff, msg_len, 0);
			if(msg_len<0)
				return 0;
		}
		free(filename);
	}else if(req_response == RP_FLIST){
		int number = extract_response_number(buff);
		Cache arr[number];
		int select = -1;
		memcpy(arr,payload,number*sizeof(Cache));
		printf("Select:\n");
		for(int i = 0;i<number;i++){
			printf("\nClient ID: %s | File name: %s",arr[i].uid_hash,arr[i].file_name);
		}
		scanf("%d",&select);
		while(getchar()!='\n');
		memcpy(payload,arr+select,sizeof(Cache));
		wrap_packet(buff,payload,sizeof(Cache),RQ_DL);
	}else if(req_response == NOTI_INF){
		fprintf(stderr,"\nServer noti:%s\n",payload);
	}else if(req_response == RP_NFOUND){
		fprintf(stderr,"\nServer noti:%s\n",payload);
	}else if(req_response == RP_STREAM){
		
	}
	return 1;
}