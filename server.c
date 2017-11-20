#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <ctype.h>

#define DEBUG 1
#include<transent/util.h>
#include <transent/session.h>
#include <transent/mypoll.h>



#define BACKLOG 100  	 		/* Number of allowed connections */
#define BUFF_SIZE 2048

#define DATA_PATH "./"

Session sessions[SESSIONS];

/* Process command from client end return */
void process(struct pollfd *po);

/* Close a connection */
void closeConnection (struct pollfd *po, Session *ss);

/* Check arguments is valid or not */
void validArguments (int argc, char *argv[], int *port);

int main(int argc, char *argv[]) {
	int port = 0;
	// checking arguments
	validArguments(argc, argv, &port);

	int listenfd, *connfd;
	struct sockaddr_in server; 		/* server's address information */
	struct sockaddr_in *client; 	/* client's address information */
	int sin_size;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;       
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */

	if(bind(listenfd,(struct sockaddr*)&server, sizeof(server))==-1){ 
		perror("\nError: ");
		return 0;
	}     

	if(listen(listenfd, BACKLOG) == -1){  
		perror("\nError: ");
		return 0;
	}
	
	sin_size = sizeof(struct sockaddr_in);
	
	/* Init listenfd */
	initPolls();

	/* Init connection listener */
	polls[0].fd = listenfd;
	polls[0].events = POLLIN;

	/* Init sessions */
	initSessions();
	
	int revents;
	
	while(1){
		revents = poll(polls, POLLS, 100000);		/* Timeout = 10s */
		if (revents > 0) {
			/* Process new connection */
			if (polls[0].revents & POLLIN) {
				connfd = (int*) malloc(sizeof(int));
				client = (struct sockaddr_in *) malloc(sin_size);
				
				if ((*connfd = accept(listenfd, (struct sockaddr *)client, &sin_size)) ==- 1)
					perror("\nError: ");

				/* Insert to polls */
				if (addPoll(*connfd, POLLIN) == 0) {
					/* polls is full -> close connection */
					close(*connfd);
					continue;
				}

				/* Insert to sessions */
				if (newSession(client, *connfd) == 0) {
					printf("Error: Number of sessions is maximum!");
					continue;
				}
				printf("You got a connection from %s\n", inet_ntoa(client->sin_addr)); /* prints client's IP */
			}

			/* Process transfer */
			for (int i = 1; i < POLLS; i++) {
				if (polls[i].fd == -1) continue;

				if (polls[i].revents & POLLIN) {
					process(polls + i);
				}
			}
		}
	}
	
	close(listenfd);
	return 0;
}

void process(struct pollfd *po) {
	int connfd = po->fd;
	Session *ss = findSessionByConnfd(connfd);
	struct sockaddr_in *client = ss->cliaddr;
	int bytes_sent, bytes_received, bytes_output;
	char recv_data[BUFF_SIZE];
	char sent_data[BUFF_SIZE];

	bytes_received = recv(connfd, recv_data, BUFF_SIZE - 1, 0); // blocking
	if (bytes_received < 0) {
		perror("\nError: ");
		closeConnection(po, ss);
	} else if (bytes_received == 0) {
		printf("Connection closed.\n");
		closeConnection(po, ss);
	} else {
		recv_data[bytes_received] = '\0';
		printf("\nReceive: |%s|\n", recv_data);

		/* Print list of client has file */

		/* Send for all other client */
		
		// Using pthread???
		for (int i = 0; i < SESSIONS; i++) {
			if (sessions[i].connfd != -1 && !isSameSession(sessions + i, ss)) {
				bytes_sent = send(sessions[i].connfd, recv_data, bytes_received, 0);
				if (bytes_sent < 0)
					perror("\nError: ");
			}
		}
		
		/* Recv from all other client */
		// for (int i = 0; i < SESSIONS; i++) {
		// 	if (sessions[i].connfd != -1 && !isSameSession(sessions + i, ss)) {

		// 	}
		// }
	}
}

void closeConnection (struct pollfd *po, Session *ss) {
	/* Close connection */
	close(po->fd);

	/* Remove from sessions */
	if (removeSession(ss) == 0) {
		printf("Error: Can't remove session because don't exist session!\n");
	}

	/* Remove from polls */
	if (removePoll(po) == 0) {
		printf("Error: Can't remove poll because don't exist poll!\n");
	}
}

void validArguments (int argc, char *argv[], int *port) {
	if (argc > 1) {
		// Check valid port
		int i;
		char *port_str = argv[1];
		for (i = 0; port_str[i] != '\0'; i++) {
			if (!isdigit(port_str[i])) {
				printf("Port is invalid\n");
				exit(EXIT_FAILURE);
			}
		}
		if (port_str[i] == '\0') *port = atoi(port_str);
	} else {
		printf("(ERROR) To few arguments!\n");
		exit(EXIT_FAILURE);
	}
}
