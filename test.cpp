#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

int main (){
	struct addrinfo ai;
	ai.ai_flags=AI_PASSIVE;
	ai.ai_family=AF_UNSPEC;
	ai.ai_socktype=SOCK_STREAM;

	struct sockaddr_storage conns;
	socklen_t conn_size;
	
	struct addrinfo *res;

	int outer=getaddrinfo(NULL,"8080",&ai,&res);
	struct sockaddr_in *adr = ( struct sockaddr_in *)res->ai_addr;

	// char ipstr[INET6_ADDRSTRLEN];
	// inet_ntop(res->ai_family,&(adr->sin_addr), ipstr, sizeof ipstr);
	int s=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	int b=bind(s,res->ai_addr,res->ai_addrlen);
	int l=listen(s,20);
	std::cout<<"Listening on port 8080";
	conn_size=sizeof conns;
	int newfd=accept(s, (struct sockaddr *)&conns, &conn_size);
	const char *msg="Hello ji";
	void *buf;
	while(int recvd=recv(newfd,&buf, 1000, 0) >0){
		//int sentb=send(newfd,&msg,strlen(msg),0);
		printf("%s\n",(char *)&buf);

		//std::cout<<sentb<<recvd;
	}
	
	
	return 0;
}