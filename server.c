#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define DEBUG 1
#include <transent/util.h>
#include <transent/session.h>
#include <transent/mypoll.h>
#include <transent/interface.h>
#include <transent/tsfmanage.h>

#define BACKLOG 100  	 		/* Number of allowed connections */
#define DATA_PATH "./db"
#define SESSIONS 1000

#define ERR_FNF "Can't find any once client contain this file!"

Session sessions[SESSIONS];
Queue *req_queue = NULL;
CacheList* cache_list = NULL;
/* Process command from client end return */
void process(struct pollfd *po);

/* Close a connection */
void closeConnection (struct pollfd *po, Session *ss);

/* Check arguments is valid or not */
void validArguments (int argc, char *argv[], int *port);
void sent_to_others(Session* ss, char* buff);
void process_find_file(Session* ss, char* buff, char* payload, int paylen);
void process_file_founded(Session* ss, char* buff, char* payload, int paylen);
void process_file_not_found(Session* ss, char* buff, char* payload, int paylen);

int main(int argc, char *argv[]) {
	int port = 0;
	// checking arguments
	validArguments(argc, argv, &port);
	init_cache_context(&cache_list);
	int listenfd, *connfd;
	struct sockaddr_in server; 		/* server's address information */
	struct sockaddr_in *client; 	/* client's address information */
	socklen_t sin_size;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;       
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

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
	initSessions(sessions,SESSIONS);
	
	int revents;
	while(1){
		revents = poll(polls, POLLS, 10000);		/* Timeout = 10s */
		if (revents > 0) {
			if (polls[0].revents & POLLIN) {
				connfd = (int*) malloc(sizeof(int));
				client = (struct sockaddr_in *) malloc(sin_size);
				
				if ((*connfd = accept(listenfd, (struct sockaddr *)client, &sin_size)) ==- 1)
					perror("\nError: ");

				if (addPoll(*connfd, POLLIN) == 0) {
					close(*connfd);
					continue;
				}

				if (newSession(client, *connfd,sessions,SESSIONS) == 0) {
					printf("Error: Number of sessions is maximum!");
					continue;
				}else{
					loginfo("New session added!");
				}
				printf("You got a connection from %s\n", 
							inet_ntoa(client->sin_addr));
			}
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
	int bytes_sent		= 0, 
		bytes_received 	= 0,
	 	bytes_output	= 0,
	 	payload_size	= 0;

	char buff[BUFF_SIZE];
	char payload[PAY_LEN];
	
	int connfd = po->fd;
	Session *ss = findSessionByConnfd(connfd,sessions,SESSIONS);
	struct sockaddr_in *client = ss->cliaddr;
	
	bzero(buff,BUFF_SIZE);
	bytes_received = recv(connfd, buff, BUFF_SIZE - 1, 0);
	int req_method = UNDEFINE;

	if (bytes_received < 0) {
		perror("\nError: ");
		closeConnection(po, ss);
	} else if (bytes_received == 0) {
		printf("Connection closed.\n");
		closeConnection(po, ss);
	} else {
		req_method = parse_packet(buff,payload,&payload_size);

		if(req_method == RQ_FILE)
			process_find_file(ss,buff,payload,payload_size);
		else if(req_method == RP_FOUND)
			process_file_founded(ss,buff,payload,payload_size);
		else if(req_method == RP_NFOUND)
			process_file_not_found(ss,buff,payload,payload_size);
		else
			printf("None!\n");
	}
}

void closeConnection(struct pollfd *po, Session *ss) {
	close(po->fd);
	if (removeSession(ss->id,sessions,SESSIONS) == 0) {
		printf("Error: Can't remove session because don't exist session!\n");
	}
	if (removePoll(po) == 0) {
		printf("Error: Can't remove poll because don't exist poll!\n");
	}
}

void validArguments (int argc, char *argv[], int *port) {
	if (argc > 1) {
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


void process_find_file(Session* ss, char* buff, char* payload, int paylen){
	int bytes_sent = -1;
	printf("on_%s\n",__func__);
	if(session_size()>1){
		Request* request = make_request(ss,payload,0);
		//Add request find file to waitting queue
		enqueue(&req_queue,request);
		wrap_packet(buff,payload,paylen,RQ_FILE);
		sent_to_others(ss,buff);
	}else{
		bytes_sent = wrap_packet(buff,ERR_FNF,strlen(ERR_FNF),NOTI_INF);
		bytes_sent = send(ss->connfd,buff, bytes_sent, 0);
		if (bytes_sent < 0)
			perror("\nError: ");
	}
}

void process_file_founded(Session* ss, char* buff, char* payload, int paylen){
	printf("on_%s\n",__func__);
	Cache* cache = new_cache(payload,ss->id);
	packet_info(buff);
	if(!cache_contain(cache_list,cache))
		push_cache(cache_list,cache);
	
	printf("Cache size:%d\n",get_all_cache_size());
}


void process_file_not_found(Session* ss, char* buff, char* payload, int paylen){
	printf("on_%s\n",__func__);
	//Todo: When file not found
}

void sent_to_others(Session* ss,char* buff){
	int bytes_sent = 0;
	printf("on_%s\n",__func__);
	for (int i = 0; i < SESSIONS; i++){
		if (sessions[i].connfd != -1 && !isSameSession(sessions + i, ss)) {
			bytes_sent = get_real_len(buff);
			bytes_sent = send(sessions[i].connfd,buff, bytes_sent, 0);
			if (bytes_sent < 0)
				perror("\nError: ");
		}
	}
}

int check_exist_file(Request* request, CacheList* cache_list){
	
}

// typedef struct _request{
//     //Request session
//     Session *session;
//     //Request file name
//     char* file_name;
//     //Timeout in milisecond
//     int timeout;
// }Request;

// typedef struct _cache{
//     char file_name[CFN_LEN];
//     char uid_hash[UID_HASH_LEN];
// }Cache;