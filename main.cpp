#include "mncproject.h"
#define arg_no 3
;char port[10];
char role;
char *myip;
char *myport;
char *serverip="localhost";
char *serverport="8080";
int connlist[10];

struct addrinfo ai;
struct sockaddr_storage conns;
struct addrinfo *res;
int listenfd;
int maxsock;
fd_set fdreads,fdmain,fdwrites;
socklen_t conn_size;

char buf[100];
// prototypes
int handleNewConnection();
void redoFDSET();
void getData(int);
void registerWithServer();
//

void handleCommand(char cmd[100]){
	printf("->%s",cmd);
	if(!strcmp(cmd, "help")){
		printf("\nAvailable Commands\n1- HELP (List Commands)\n2- MYIP (Show My IP Address)\n3- MYPORT (Show My Port)\n4- REGISTER <server IP><port no> ()\n5- CONNECT <destination><port> ()\n6- LIST (List all connections with details)\n7- TERMINATE <connection_id> (Terminate a connection)\n8- EXIT (exit the process)\n9- UPLOAD <connection_id> <filename> (upload this file)\n10-DOWNLOAD <connection id 1 ><file><connection id 2><file2> <connection id 3><file3>\n11-Creator of this software\n\n");
		//handleCommand();
	}
	else if(!strcmp(cmd, "myip")){

	}
	else if(!strcmp(cmd, "myport")){
		printf("\nMy current Port is %s\n", port);
		//handleCommand();
	}
	else if(!strcmp(cmd, "register")){
		if(role=='c'){
			registerWithServer();
		}
		else{
			printf("You need to be a client to use this command\n");
		}
	}
	else if(!strcmp(cmd, "connect")){
		
	}
	else if(!strcmp(cmd, "list")){
		
	}
	else if(!strcmp(cmd, "terminate")){
		
	}
	else if(!strcmp(cmd, "exit")){
		printf("Exiting the Program, Bye!\n");
		close(listenfd);
		exit(1);
	}
	else if(!strcmp(cmd, "upload")){
		
	}
	else if(!strcmp(cmd, "download")){
		
	}
	else if(!strcmp(cmd, "creator")){
		printf("\nCreated by - %s\nUBIT - %s\nEmail - %s\n\n","Karnesh Mehra","karneshm","karneshm@buffalo.edu" );
		//handleCommand();
	}
	else{
		printf("Command Not Found\n");
		//handleCommand();
	}
	//printf("The commnad is %s\n", cmd);
}

int makeServer(){
	memset(&ai, 0, sizeof ai);
	ai.ai_flags=AI_PASSIVE;
	ai.ai_family=AF_UNSPEC;
	ai.ai_socktype=SOCK_STREAM;

	int outer=getaddrinfo(NULL,port,&ai,&res);
	//printf("%d\n", outer);
	struct sockaddr_in *adr = ( struct sockaddr_in *)res->ai_addr;

	// char ipstr[INET6_ADDRSTRLEN];
	// inet_ntop(res->ai_family,&(adr->sin_addr), ipstr, sizeof ipstr);
	listenfd=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	int optval=1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	maxsock=listenfd;
	int b=bind(listenfd,res->ai_addr,res->ai_addrlen);
	if(b==-1){
		printf("%s\n", strerror(errno));
	}
    int l=listen(listenfd,10);
	printf("__%d__%d__%d\n", listenfd,b,l);	
    freeaddrinfo(res);
	struct timeval t ;
	t.tv_sec=5;
	t.tv_usec=0;


    while(1){
    	printf("%d**%d**%s\n",maxsock,listenfd,buf);
   		redoFDSET();
    	int socksel=select(maxsock+1, &fdreads, NULL, NULL, &t);
    	printf("2-%d\n",socksel);
    	if(socksel==-1){
    		perror("There has been select error");
    	}

    	if(socksel==0){
    		printf("No nthing\n");
    	}
    	if(socksel>0){
    		if(FD_ISSET(listenfd, &fdreads)){
    			handleNewConnection();
	    	}
	    	if(FD_ISSET(0,&fdreads)){
	    		int len;
	    		char readbuff[100];
	    		len=read(0,readbuff,sizeof readbuff);
	    		readbuff[len-1]='\0';
	    		if(len>0){
	    			//int a=strcmp("hello",readbuff);
	    			//printf("%d-%s]]",a,readbuff);
	    			handleCommand(readbuff);
	    		}
	    	}
	    	for(int i=0;i<10;i++){
	    		if(connlist[i]!=-2){
	    			printf("check for data--%d\n",i);
	    			if(FD_ISSET(connlist[i], &fdreads)){
	    				printf("Data incoming\n");
	    				getData(connlist[i]);
	    			}
	    		}
	    	}
    	}

    	
    }
	return 0;
}

void redoFDSET(){
	FD_ZERO(&fdreads);
    FD_SET(listenfd, &fdreads);
    FD_SET(0, &fdreads);
	for(int i=0;i<10;i++){
		if(connlist[i]!=-2){
			FD_SET(connlist[i], &fdreads);
		}
	}
}

void getData(int fd){
	//char buf[100];
	int data=read(fd,buf,sizeof(buf));
	
		printf("%d--data-%s\n",data,buf);
		memset(buf, 0, sizeof(buf));
}

int makeClient(){
	ai.ai_flags=AI_PASSIVE;
	ai.ai_family=AF_UNSPEC;
	ai.ai_socktype=SOCK_STREAM;

	int outer=getaddrinfo(NULL,port,&ai,&res);
	//printf("%d\n", outer);
	struct sockaddr_in *adr = ( struct sockaddr_in *)res->ai_addr;

	// char ipstr[INET6_ADDRSTRLEN];
	// inet_ntop(res->ai_family,&(adr->sin_addr), ipstr, sizeof ipstr);
	listenfd=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	maxsock=listenfd;
	int b=bind(listenfd,res->ai_addr,res->ai_addrlen);
    int l=listen(listenfd,10);
	

	struct timeval t ;
	t.tv_sec=5;
	t.tv_usec=0;


    while(1){
    	printf("%d**%d\n",maxsock,listenfd);
   		redoFDSET();
    	int socksel=select(maxsock+1, &fdreads, NULL, NULL, &t);
    	printf("2-%d\n",socksel);
    	if(socksel==-1){
    		perror("There has been select error");
    	}

    	if(socksel==0){
    		printf("No nthing\n");
    	}
    	if(socksel>0){
    		if(FD_ISSET(listenfd, &fdreads)){
    			handleNewConnection();
	    	}
	    	if(FD_ISSET(0,&fdreads)){
	    		int len;
	    		char readbuff[100];
	    		len=read(0,readbuff,sizeof(readbuff));
	    		readbuff[len-1]='\0';
	    		if(len>0){
	    			//int a=strcmp("hello",readbuff);
	    			//printf("%d-%s]]",a,readbuff);
	    			handleCommand(readbuff);
	    		}
	    	}
	    	// for(int i=0;i<10;i++){
	    	// 	if(connlist[i]!=-2){
	    	// 		printf("check for data--%d\n",i);
	    	// 		if(FD_ISSET(connlist[i], &fdreads)){
	    	// 			printf("Data incoming\n");
	    	// 			getData(connlist[i]);
	    	// 		}
	    	// 	}
	    	// }
    	}

    	
    }
	return 0;
}

void addToConnList(int fd){
	for(int i=0;i<10;i++){
    	if(connlist[i]==-2){
    		connlist[i]=fd;
    		printf("%d=added to conn list==%d\n",i,fd);
    		if(fd>maxsock){
    			printf("max bhi tha\n");
    			maxsock=fd;
    		}
    		break;
    	}
    }
}

int handleNewConnection(){
    printf("THere is a new connection\n");
    conn_size=sizeof conns;
    int newfd=accept(listenfd,(struct sockaddr *)&conns, &conn_size);
    char buf[200];
    char porta[200];
    getnameinfo((struct sockaddr *)&conns, conn_size, buf, sizeof buf, porta, sizeof porta, 0);
    printf("%s--%s\n", buf,porta);
    addConnection(buf, porta);
    char *msg="got it";
    //int data=send(newfd,&msg,sizeof msg,0);
    //printf("%d\n",data );
    addToConnList(newfd);
    traverseList();
}

void registerWithServer(){
	// struct addrinfo ai1;
	// ai1.ai_flags=AI_PASSIVE;
	// ai1.ai_family=AF_UNSPEC;
	// ai1.ai_socktype=SOCK_STREAM;
	// struct addrinfo *res1;
	struct sockaddr_in servaddr,cliaddr;
	// int outer=getaddrinfo(serverip,serverport,&ai1,&res1);
	// int tmpfd=socket(res1->ai_family, res1->ai_socktype, res1->ai_protocol);
	// int connct=connect(tmpfd,res1->ai_addr, res1->ai_addrlen);
    char msg[100]="MNCOK";
	//int data=write(tmpfd,msg,strlen(msg));
	//printf("reg with server--%d--%d--%d\n", tmpfd,connct,data);
	// /close(tmpfd);

	int sockfd=socket(AF_INET,SOCK_STREAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
   int porta=8080;
   servaddr.sin_port=htons(porta);
   connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
   int data=write(sockfd,msg,strlen(msg));
	printf("reg with server--%d--%d\n", sockfd,data);
}

void boot(){
	initializeList();
	for(int i=0;i<10;i++)
		connlist[i]=-2;
}

int main ( int argc, char *argv[]){
	if(argc!=arg_no ){
		printf("%s\n","Arguments missing!");
		return 0;
	}
	else{
		boot();
		role=*argv[1];
		//port=*argv[2];
		strcpy(port, argv[2]);

		//printf("Port number is %s\n",port );
		
		if(role=='c'){
			makeClient();
			//printf("Its a client\n");
		}
		else if(role=='s'){
			makeServer();
			//printf("Its a server\n");
		}
		else{
			//printf("Unknown role\n");
			return 0;
		}
	}
	
	return 0;
}