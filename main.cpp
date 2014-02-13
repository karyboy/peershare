#include "mncproject.h"
#define arg_no 3
;char port[10];
char role;
char readbuff[26];
char *myip;
char *myport;
char *serverip="localhost";
char *serverport="8080";

struct addrinfo ai;
struct sockaddr_storage conns;
struct addrinfo *res;
int listenfd;
int maxsock;
fd_set fdreads,fdmain,fdwrites;
socklen_t conn_size;

// prototypes
int handleNewConnection();
//

void handleCommand(char cmd[20]){
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
    	printf("1\n");
    	FD_ZERO(&fdreads);
    	FD_SET(listenfd, &fdreads);
    	FD_SET(0, &fdreads);
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
	    		len=read(0,readbuff,sizeof readbuff);
	    		readbuff[len-1]='\0';
	    		if(len>0){
	    			int a=strcmp("hello",readbuff);
	    			//printf("%d-%s]]",a,readbuff);
	    			handleCommand(readbuff);
	    		}
	    	}
    	}

    	
    }
	// printf("Listening on Port %s\n",port);
	// conn_size=sizeof conns;
	// int newfd=accept(s, (struct sockaddr *)&conns, &conn_size);
	// const char *msg="Hello ji";
	// char *buf;
	// while(int recvd=recv(newfd,&buf, 1000, 0) >0){
	// 	printf("%s",(char *)&buf);
	// 	int sentb=send(newfd,&msg,strlen(msg),0);
	// }
	return 0;
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
    traverseList();
}

void boot(){
	initializeList();
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

			//handleCommand();
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