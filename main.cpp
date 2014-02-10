#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>

#define arg_no 3

int main ( int argc, char *argv[]){
	char role;   // client or server role 
    int port;	// port number 

	if(argc!=arg_no ){
		printf("%s\n","Arguments missing!");
		return 0;
	}
	else{
		role=*argv[1];
		port=atoi(argv[2]);

		printf("Port number is %d\n",port );
		
		if(role=='c'){
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