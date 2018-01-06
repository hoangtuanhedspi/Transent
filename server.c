#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>

#define DEBUG 1
#include <transent/util.h>
#include <transent/user.h>
#include <transent/authen.h>
#include <transent/session.h>
#include <transent/mypoll.h>
#include <transent/interface.h>
#include <transent/tsfmanage.h>
#include <transent/request.h>

#define BACKLOG 100  	 		/* Number of allowed connections */
#define DATA_PATH "./db"
#define USERS 100
#define SESSIONS 1000
#define USER_FILE "account.txt"

#define ERR_FNF "Can't find any once client contain this file!"

typedef struct _thread_data{
	CacheList* cache_list;
	Queue ** req_queue;
}PassData;

/* Process command from client end return */
void process(struct pollfd *po);
void sig_handler(int signum);
/* Close a connection */
void closeConnection (struct pollfd *po, Session *ss);

/* Check arguments is valid or not */
void validArguments (int argc, char *argv[], int *port);

/**
 * 
 */
void sent_to_others(Session* session, char* buff);

/**
 * 
 */
void process_find_file(session,buff,payload,paylen);

/**
 * 
 */
void process_file_founded(session,buff,payload,paylen);

/**
 * 
 */
void process_file_transfer(session,buff,payload,paylen);

/**
 * 
 */
void process_file_received(session,buff,payload,paylen);

/**
 * 
 */
void process_file_not_found(session,buff,payload,paylen);

/**
 * 
 */
void process_file_download(session,buff,payload,paylen);

/**
 * 
 */
CacheList* get_list_file(Request* request, CacheList* cache_list);

/**
 * 
 */
void* timeout_thread(void * data);

/**
 * 
 */
void retain_request_timeout(Queue* req);

void retain_cache_life_time(CacheList* list);

/**
 * 
 */
void process_request_queue(CacheList* list, Queue** req);

/**
 * 
 */
int process_request(CacheList* list, Request* req);

void processSignup (session, buff, payload, paylen);
void processLogin (session, buff, payload, paylen);
void processLogout(session, buff, payload, paylen);
void parseUserInfo(char *user_string, char *user_id, char *user_pass);
void sendMessage (int connfd, int method, char *buff, char *msg);

User users[USERS];
Session sessions[SESSIONS];
Queue *req_queue = NULL;
CacheList* cache_list = NULL;
//Thread for countdown timeout, retain request queue
pthread_t timeout;

int main(int argc, char *argv[]) {
	// Read list-user from file
	readUsers(USER_FILE, users, USERS);

	int port = 0;
	int listenfd, *connfd;
	struct sockaddr_in server;
	struct sockaddr_in *client;
	socklen_t sin_size;
    int status;
	PassData* pass_data;

	// checking arguments
	validArguments(argc, argv, &port);
	
	//Init list file cache context in server context
	init_cache_context(&cache_list);

	//Init passing data point to timeout thread
	pass_data = tsalloc(PassData,sizeof(PassData));
	bzero(pass_data,sizeof(PassData));
	pass_data->cache_list = cache_list;
	pass_data->req_queue = &req_queue;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
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

	//Create timer thread
    signal(SIGINT, sig_handler);

    status = pthread_create(&timeout, NULL, &timeout_thread, pass_data);
	if (status != 0) {
        printf("Failed to create timer thread with status = %d\n", status);
		exit(EXIT_FAILURE);
    }

	int revents;
	while(1){
		revents = poll(polls, POLLS, 10000);			//Recall for polling new events
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
	pthread_exit(NULL);
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
		writeUsers(USER_FILE, users, USERS);
		closeConnection(po, ss);
	} else {
		req_method = parse_packet(buff,payload,&payload_size);
		switch (req_method) {
		case RQ_FILE:
			process_find_file(ss,buff,payload,payload_size);
			break;
		case RQ_DL:
			process_file_download(ss,buff,payload,payload_size);
			break;
		case RP_FOUND:
			process_file_founded(ss,buff,payload,payload_size);
			break;
		case RP_NFOUND:
			process_file_not_found(ss,buff,payload,payload_size);
			break;
		case RP_STREAM:
			process_file_transfer(ss,buff,payload,payload_size);
			break;
		case RQ_STREAM:
			process_file_received(ss,buff,payload,payload_size);
			break;
		case RQ_SIGNUP:
			processSignup(ss, buff, payload, payload_size);
			break;
		case RQ_LOGIN:
			processLogin(ss, buff, payload, payload_size);
			break;
		case RQ_LOGOUT:
			processLogout(ss, buff, payload, payload_size);
			break;
		default:
			printf("None!\n");
		}
	}
}

void processSignup (session, buff, payload, paylen)
Session* session;char* buff;char* payload;int paylen;
{
	char user_id[USER_ID_LEN + 1] = "";
	char user_pass[PASS_LEN + 1] = "";

	parseUserInfo(payload, user_id, user_pass);
	printf("user = |%s|\n", user_id);
	printf("pass = |%s|\n", user_pass);

	if (user_id[0] == '\0' || user_pass == '\0') {
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Signup fail! Please using command \'SIGNUP user@password\'.\x1B[0m");
		return;
	}

	enum NewUserState new_user_state = newUser(createUser(user_id, user_pass), users, USERS);

	switch (new_user_state) {
	case SUCCESS:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[32m=> Signup successful! Please using \'LOGIN user@password\' to login.\x1B[0m");
		break;
	case EXISTED:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Signup fail! This user existed.\x1B[0m");
		break;
	case FULL:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Signup fail! Can't create new users.\x1B[0m");
		break;
	default:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Logout fail! Something wrong.\x1B[0m");
	}
}

void processLogout(session, buff, payload, paylen)
Session* session;char* buff;char* payload;int paylen;
{
	char user_id[USER_ID_LEN + 1] = "";
	char user_pass[PASS_LEN + 1] = "";

	parseUserInfo(payload, user_id, user_pass);

	if (user_id[0] == '\0' || user_pass == '\0') {
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Logout fail! Please using command \'LOGOUT user@password\'.\x1B[0m");
		return;
	}

	enum LogoutState logout_state = logout(session->connfd, user_id, user_pass, sessions, SESSIONS, users, USERS);

	switch (logout_state) {
	case LO_SUCCESS:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[32m=> Logout successful!\x1B[0m");
		break;
	case LO_NOT_IDENTIFIED_USER:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Logout fail! Don't exist any user in session.\x1B[0m");
		break;
	case LO_WRONG_USER:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Logout fail! UserID doesn't match.\x1B[0m");
		break;
	case LO_WRONG_PASS:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Logout fail! Wrong password.\x1B[0m");
		break;
	case LO_USER_BLOCKED:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Logout fail! User was blocked.\x1B[0m");
		break;
	default:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Logout fail! Something wrong.\x1B[0m");
	}
}

void processLogin (session, buff, payload, paylen)
Session* session;char* buff;char* payload;int paylen;
{
	char user_id[USER_ID_LEN + 1] = "";
	char user_pass[PASS_LEN + 1] = "";

	parseUserInfo(payload, user_id, user_pass);

	if (user_id[0] == '\0' || user_pass == '\0') {
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Login fail! Please using command \'LOGIN user@password\'.\x1B[0m");
		return;
	}

	enum LoginUserState login_user_state = loginUser(session->connfd, user_id, sessions, SESSIONS, users, USERS);

	switch (login_user_state) {
	case LU_SUCCESS: {
		enum LoginPassState login_pass_state = loginPass(session->connfd, user_pass, sessions, SESSIONS, users, USERS);

		if (login_pass_state == LP_SUCCESS) {
			// Login success
			sendMessage(session->connfd, RP_MSG, buff, "\x1B[32m=> Login successful!\x1B[0m");
		} else {
			// Wrong pass
			sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Login fail! Password is wrong.\x1B[0m");
			removeUserFromSession(session);
		}
		break;
	}
	case LU_EXISTED_ONE_USER:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Login fail! Existed another user.\x1B[0m");
		break;
	case LU_USER_BLOCKED:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Login fail! User is blocked.\x1B[0m");
		break;
	case LU_NOT_FOUND:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Login fail! User not found.\x1B[0m");
		break;
	default:
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Login fail! Something wrong.\x1B[0m");
	}

	printf("Session state: %d\n", session->state);
}

void parseUserInfo (char *user_string, char *user_id, char *user_pass) {
	char *flag = strchr(user_string, '@');
	if (flag == NULL) {
		strcpy(user_id, "");
		strcpy(user_pass, "");
	} else {
		strncpy(user_id, user_string, flag - user_string);
		strcpy(user_pass, flag + 1);
	}
}

void sendMessage (int connfd, int method, char *buff, char *msg) {
	add_request(buff, method);
	attach_payload(buff, msg, strlen(msg));

	packet_info(buff);

	int msg_len = get_real_len(buff);
	int bytes_sent = send(connfd, buff, msg_len, 0);
	if(bytes_sent <= 0){
		printf("\nConnection closed!\n");
		return 0;
	}
}

void closeConnection(struct pollfd *po, Session *session) {
	close(po->fd);
	if (removeSession(session->id,sessions,SESSIONS) == 0) {
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


void process_find_file(session,buff,payload,paylen)
Session* session;char* buff;char* payload;int paylen;
{
	printf("on_%s\n",__func__);
	int bytes_sent = -1;
	if(session->state == AUTHENTICATED){
		if(session_size()>1){
			Request* request = make_request(session,payload,DEFAULT_TIMEOUT);
			//Add request find file to waitting queue
			enqueue(&req_queue,request);
			//Send request find file to all client
			wrap_packet(buff,payload,paylen,RQ_FILE);
			sent_to_others(session,buff);
		}else{
			bytes_sent = wrap_packet(buff,ERR_FNF,strlen(ERR_FNF),NOTI_INF);
			bytes_sent = send(session->connfd,buff, bytes_sent, 0);
			if (bytes_sent < 0)
				perror("\nError: ");
		}
	}else{
		sendMessage(session->connfd, RP_MSG, buff, "\x1B[31m=> Please login to find file.\x1B[0m");
	}
}

void process_file_founded(session,buff,payload,paylen)
Session* session;char* buff;char* payload;int paylen;
{
	printf("on_%s\n",__func__);
	if(session->state = AUTHENTICATED){
		Cache* cache = new_cache(payload,session->id);
		if(!cache_contain(cache_list,cache))
			push_cache(cache_list,cache);
	}
}


void process_file_not_found(session,buff,payload,paylen)
Session* session;char* buff;char* payload;int paylen;
{
	printf("on_%s\n",__func__);
	//Todo: When file not found
}

void process_file_transfer(session,buff,payload,paylen)
Session* session;char* buff;char* payload;int paylen;
{
	printf("on_%s\n",__func__);
	//RP_STREAM
	int req_response = UNDEFINE, bytes_transfer = 0;
	//Get meta data from header
	char* file_name = get_meta_data(buff);
	int uid = extract_response_number(buff);
	char id[UID_HASH_LEN];
	bzero(id,UID_HASH_LEN);
	sprintf(id,"%d",uid);
	Session * s = findSessionById(id,sessions,SESSIONS);
	if(!s) printf("NULL\n");
	add_response_number(buff,atoi(session->id));
	add_request(buff,RP_STREAM);
	bytes_transfer = get_real_len(buff);
	bytes_transfer = send(s->connfd,buff, bytes_transfer, 0);
	if (bytes_transfer < 0)
		perror("\nError");
}

void process_file_received(session,buff,payload,paylen)
Session* session;char* buff;char* payload;int paylen;
{
	printf("on_%s\n",__func__);
	int req_response = UNDEFINE, bytes_transfer = 0;
	char* file_name = get_meta_data(buff);
	int uid = extract_response_number(buff);
	char id[UID_HASH_LEN];
	sprintf(id,"%d",uid);
	Session * s = findSessionById(id,sessions,SESSIONS);
	add_response_number(buff,atoi(session->id));
	add_request(buff,RQ_STREAM);
	bytes_transfer = get_real_len(buff);
	bytes_transfer = send(s->connfd,buff, bytes_transfer, 0);
	if (bytes_transfer < 0)
		perror("\nError");
}

void process_file_download(session,buff,payload,paylen)
Session* session;char* buff;char* payload;int paylen;
{
	printf("on_%s\n",__func__);
	int bytes_sent = 0;
	Cache* cache = tsalloc(Cache,sizeof(Cache));
	memcpy(cache,payload,sizeof(Cache));
	Session* s = findSessionById(cache->uid_hash,sessions,SESSIONS);
	strcpy(cache->uid_hash,session->id);
	memcpy(payload,cache,sizeof(Cache));
	wrap_packet(buff,payload,sizeof(Cache),RQ_DL);
	bytes_sent = get_real_len(buff);
	bytes_sent = send(s->connfd,buff, bytes_sent, 0);
	if (bytes_sent < 0)
		perror("\nError");
}

void sent_to_others(Session* session,char* buff){
	int bytes_sent = 0;
	printf("on_%s\n",__func__);
	for (int i = 0; i < SESSIONS; i++){
		if (sessions[i].connfd != -1 && 
			!isSameSession(sessions + i, session)) {
			bytes_sent = get_real_len(buff);
			bytes_sent = send(sessions[i].connfd,buff, bytes_sent, 0);
			if (bytes_sent < 0)
				perror("\nError");
		}
	}
}

int cmp_cfn(Var des,Var res){
    if(!des || !res) return 0;
    return strcmp(((Request*)des)->file_name,
				 ((Cache*)res)->file_name) == 0;
}

CacheList* get_list_file(Request* request, CacheList* cache_list){
	if(!request || !cache_list) return NULL;
	CacheList* 	tmp 	= NULL,
				*res 	= NULL;
	tmp = cache_list;
	while(tmp != NULL){
		if(cmp_cfn(request,tmp->data)!=0)
			append(&res,tmp->data);
		tmp = tmp->next;
	}
	return res;
}

void* timeout_thread(void * data){
	CacheList* list = ((PassData*)data)->cache_list;
	Queue** req = NULL;
	req = ((PassData*)data)->req_queue;
	while(1){
		retain_request_timeout(*req);
		retain_cache_life_time(list);
		process_request_queue(list,req);
		sleep(1);
	}
	pthread_exit(NULL);
}

void sig_handler(int signum) {
    if (signum != SIGINT) {
        printf("Received invalid signum = %d in sig_handler()\n", signum);
    }
    printf("Received SIGINT. Exiting threads\n");
    pthread_cancel(timeout);
	exit(EXIT_SUCCESS);
}

void retain_request_timeout(Queue* req){
	Queue* tmp = req;
	while(tmp!=NULL){
		((Request*)(tmp->data))->timeout--;
		tmp = tmp->next;
	}
}

void retain_cache_life_time(CacheList* list){
	Node* tmp = list;
	if(get_all_cache_size()>0){
		while(tmp!=NULL){
			int life = ((Cache*)(tmp->data))->life--;
			if(life==0)
				remove_cache(list,(Cache*)(tmp->data));
			tmp = tmp->next;
		}
	}
}


void process_request_queue(CacheList* list, Queue** req){
	Queue* tmp = *req;
	while(tmp!=NULL){
		int status = process_request(list,(Request*)tmp->data);
		if(status == 1)
			drop_request(req,(Request*)tmp->data);
		tmp = tmp->next;
	}
}

int process_request(CacheList* list, Request* req){
	char buff[BUFF_SIZE];
	char payload[PAY_LEN];
	bzero(buff,BUFF_SIZE);
	int bytes_sent = 0;

	if(req->timeout < 0) {
		add_request(buff,RP_NFOUND);
		bzero(payload,PAY_LEN);
		sprintf(payload,"Can't find [%s] on any device!",req->file_name);
		attach_payload(buff,payload,strlen(payload));
		bytes_sent = get_real_len(buff);
		bytes_sent = send(req->session->connfd,buff, bytes_sent, 0);
		if(bytes_sent<=0){
			perror("Error");
		}
		return 1;
	}

	//In time
	CacheList* smlist = get_list_file(req,list);
	int len = length(smlist);
	if(len>0){
		Cache* arr = list_cache_to_array(smlist);
		add_request(buff,RP_FLIST);
		add_response_number(buff,len);
		memcpy(payload,arr,len*sizeof(Cache));
		attach_payload(buff,payload,len*sizeof(Cache));
		bytes_sent = get_real_len(buff);
		bytes_sent = send(req->session->connfd,buff, bytes_sent, 0);
		if(bytes_sent<=0){
			perror("Error");
		}
		return 1;
	}

	return 0;
}