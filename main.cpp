#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>

#define arg_no 3
int port;
char role;

void handleCommand(){
	char cmd[20];
	printf("->");
	scanf("%s",cmd);
	if(!strcmp(cmd, "help")){
		printf("\nAvailable Commands\n1- HELP (List Commands)\n2- MYIP (Show My IP Address)\n3- MYPORT (Show My Port)\n4- REGISTER <server IP><port no> ()\n5- CONNECT <destination><port> ()\n6- LIST (List all connections with details)\n7- TERMINATE <connection_id> (Terminate a connection)\n8- EXIT (exit the process)\n9- UPLOAD <connection_id> <filename> (upload this file)\n10-DOWNLOAD <connection id 1 ><file><connection id 2><file2> <connection id 3><file3>\n11-Creator of this software\n\n");
		handleCommand();
	}
	else if(!strcmp(cmd, "myip")){

	}
	else if(!strcmp(cmd, "myport")){
		printf("\nMy current Port is %d\n", port);
		handleCommand();
	}
	else if(!strcmp(cmd, "register")){
		
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
		handleCommand();
	}
	else{
		printf("Command Not Found\n");
		handleCommand();
	}
	//printf("The commnad is %s\n", cmd);
}

int main ( int argc, char *argv[]){
	if(argc!=arg_no ){
		printf("%s\n","Arguments missing!");
		return 0;
	}
	else{
		role=*argv[1];
		port=atoi(argv[2]);

		printf("Port number is %d\n",port );
		
		if(role=='c'){
			handleCommand();
			printf("Its a client\n");
		}
		else if(role=='s'){
			printf("Its a server\n");
		}
		else{
			printf("Unknown role\n");
			return 0;
		}
	}
	
	return 0;
}