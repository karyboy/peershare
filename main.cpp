#include "mncproject.h"
#define arg_no 3
;

int handleNewConnection();
void redoFDSET();
void getData(int);
void registerWithServer(string,string);
void assignMaxFD();
void sendMsg(string,string,string);
void sendCnxnList();
//

void handleCommand(char ccmd[100]){
	std::string str=std::string(ccmd);
	std::vector<std::string> tokens=tokenize(str," ");
	std::string cmd=tokens[0];
	//split(tokens,cmd,is_any_of(" "));
	if(cmd.compare("help")==0){
		printf("\nAvailable Commands\n1- HELP (List Commands)\n2- MYIP (Show My IP Address)\n3- MYPORT (Show My Port)\n4- REGISTER <server IP><port no> ()\n5- CONNECT <destination><port> ()\n6- LIST (List all connections with details)\n7- TERMINATE <connection_id> (Terminate a connection)\n8- EXIT (exit the process)\n9- UPLOAD <connection_id> <filename> (upload this file)\n10-DOWNLOAD <connection id 1 ><file><connection id 2><file2> <connection id 3><file3>\n11-Creator of this software\n\n");
		//handleCommand();
	}
	else if(cmd.compare("myip")==0){
		cout<<"My ip address is "<<myip<<"\n";
	}
	else if(cmd.compare("myport")==0){
		printf("\nMy current Port is %s\n", port);
		//handleCommand();
	}
	else if(cmd.compare("register")==0){
		if(role=='c'){
			if(tokens[1].length()>0 && tokens[2].length()>0)
				registerWithServer(tokens[1],tokens[2]);
			else
				cout<<"Parameters Missing\n";
		}
		else{
			printf("You need to be a client to use this command\n");
		}
	}
	else if(cmd.compare("connect")==0){
		
	}
	else if(cmd.compare("list")==0){
		traverseConnections();
	}
	else if(cmd.compare("terminate")==0){
		tester();
	}
	else if(cmd.compare("exit")==0){
		printf("Exiting the Program, Bye!\n");
		close(listenfd);
		exit(1);
	}
	else if(cmd.compare("upload")==0){
		
	}
	else if(cmd.compare("download")==0){
		
	}
	else if(cmd.compare("creator")==0){
		printf("\nCreated by - %s\nUBIT - %s\nEmail - %s\n\n","Karnesh Mehra","karneshm","karneshm@buffalo.edu" );
		//handleCommand();
	}
	else{
		printf("Command Not Found\n");
		//handleCommand();
	}
	//printf("The commnad is %s\n", cmd);
}

int listener(){
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
		printf("error hai- %s\n", strerror(errno));
	}
    int l=listen(listenfd,10);
	printf("__%d__%d__%d\n", listenfd,b,l);	
    freeaddrinfo(res);
	struct timeval t ;
	t.tv_sec=5;
	t.tv_usec=0;


    while(1){
    	//printf("%d**%d**%s\n",maxsock,listenfd,buf);
   		redoFDSET();
    	int socksel=select(maxsock+1, &fdreads,&fdwrites, NULL, NULL);
    	//printf("2-%d\n",socksel);
    	if(socksel==-1){
    		perror("There has been select error");
    	}

    	if(socksel==0){
    		//printf("No nthing\n");
    	}
    	if(socksel>0){
    		if(FD_ISSET(listenfd, &fdreads)){
    			handleNewConnection();
	    	}
	    	if(FD_ISSET(0,&fdreads)){
	    		int len;
	    		char readbuff[100];
	    		len=read(0,readbuff,sizeof readbuff);
	    		//fgets(readbuff, sizeof readbuff, 0);
	    		readbuff[len-1]='\0';
	    		//printf("%s\n", readbuff);
	    		if(len>0){
	    			if(readbuff[0]=='\n')
	    				printf("New line\n" );
	    			//int a=strcmp("hello",readbuff);
	    			//printf("%s]]",len[]);
	    			handleCommand(readbuff);
	    		}
	    	}
	    	if(FD_ISSET(1, &fdwrites)){
	    		fflush(stdout);
	    	}
	    	for(int i=0;i<10;i++){
	    		if(connlist[i]!=-2){
	    			//printf("check for data--%d\n",i);
	    			if(FD_ISSET(connlist[i], &fdreads)){
	    				printf("Data incoming\n");
	   					//FD_CLR(connlist[i], &fdreads);
	   					getData(connlist[i]);
	   					connlist[i]=-2;
	   					assignMaxFD();
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
    FD_SET(1, &fdwrites);
	for(int i=0;i<10;i++){
		if(connlist[i]!=-2){
			FD_SET(connlist[i], &fdreads);
		}
	}
}

void getData(int fd){
	int data=read(fd,buf,sizeof(buf));
	printf("%d--data-%s\n",data,buf);
	string cmd=string(buf);
	//cout<<"Cmd is "<<cmd<<"\n";
	if(cmd.find("reg")!=string::npos){
		string p=cmd.substr(cmd.find("_")+1,cmd.find("|")-cmd.find("_")-1);
		string ip=cmd.substr(cmd.find("|")+1,cmd.length()-cmd.find("|"));
		cout<<"port is"<<p<<endl;
		//cout<<"ip is "<<myip<<endl;
    	addConnection(ip, p);
    	memset(buf, 0, sizeof(buf));
    	close(fd);
    	FD_CLR(fd, &fdreads);
    	traverseConnections();
    	sendCnxnList();
		//updatePort(string id, string port)
	}
	else if(cmd.find("peers")!=string::npos){
		string p=cmd.substr(cmd.find("_")+1,cmd.length());
    	//cout<<"\nyooooooo-"<<p<<"\n";
    	emptyConnections();
    	formPeerVector(p);
    	memset(buf, 0, sizeof(buf));
    	close(fd);
    	FD_CLR(fd, &fdreads);
    	traverseConnections();
    	//updatePort(string id, string port)
	}
	else{
		cout<<"no relevant data\n";
		memset(buf, 0, sizeof(buf));
    	close(fd);
    	FD_CLR(fd, &fdreads);
	}
}

void assignMaxFD(){
	for(int i;i<10;i++){
		if(connlist[i]>maxsock )
			maxsock=connlist[i];
	}
	if(listenfd>maxsock)
		maxsock=listenfd;
}

void addToConnList(int fd){
	for(int i=0;i<10;i++){
    	if(connlist[i]==-2){
    		connlist[i]=fd;
    		//printf("%d=added to conn list==%d\n",i,fd);
    		if(fd>maxsock){
    			//printf("max bhi tha\n");
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
    //inet_ntop(AF_INET, (struct sockaddr_in *)&conns, buf, sizeof(buf));
    //getnameinfo((struct sockaddr *)&conns, conn_size, buf, sizeof buf, porta, sizeof porta, 0);
    //printf("{{%s}}\n",buf);
    // addConnection(string(buf),string(porta));
    addToConnList(newfd);
}

void registerWithServer(string ipaddr,string porta){
   serverip=ipaddr;
   serverport=porta;
   struct addrinfo hints, *re;
   memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags=AI_PASSIVE;
   int outa=getaddrinfo(ipaddr.c_str(), porta.c_str(), &hints, &re);
   //cout<<outa<<"}}}}}\n";
   //struct sockaddr_in servaddr;
   string msg="reg_"+string(port)+"|"+myip;
   int sockfd=socket(re->ai_family,re->ai_socktype,re->ai_protocol);
   //int sockfd=socket(AF_INET,SOCK_STREAM,0);
   //bzero(&servaddr,sizeof(servaddr));
   //int optval=1;
   //setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
   //servaddr.sin_family = AF_INET;
   // servaddr.sin_addr.s_addr=inet_addr(ipaddr.c_str());
   // servaddr.sin_port=htons(atoi(porta.c_str()));
   int conn=connect(sockfd, re->ai_addr, re->ai_addrlen);
   //int conn=connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
   if(conn>-1){
   		int data=write(sockfd,msg.c_str(),strlen(msg.c_str()));
	   printf("reg with server--%d--%d\n", strlen(msg.c_str()),data);
	   shutdown(sockfd, SHUT_WR);
	   char closebuf[100];
	   while(1){
	   		int r=read(sockfd,closebuf,sizeof(closebuf));
	   		cout<<"~~"<<r<<"~~";
	   		if(!r)
	   			break;
	   }
	   close(sockfd);	
   }
   else{

   		cout<<"Connection error -- "<<strerror(errno);
   		close(sockfd);
   }
   
   
}

void sendCnxnList(){
	string str=formPeerString();
	for(int i=0;i<connections.size();i++){
		vector<string> tmp=connections[i];
		sendMsg(tmp[1], tmp[2], str);
	}
}

void sendMsg(string ipaddr,string porta,string msg){
	//cout<<"("<<ipaddr<<"-"<<porta<<"-"<<msg<<"-\n";
	struct sockaddr_in servaddr,cliaddr;
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    int optval=1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(ipaddr.c_str());
    servaddr.sin_port=htons(atoi(porta.c_str()));
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    int data=write(sockfd,msg.c_str(),strlen(msg.c_str()));
    //printf("sending msgs --%d--%d\n", strlen(msg.c_str()),data);
    shutdown(sockfd, SHUT_WR);
    char closebuf[100];
	   while(1){
	   		int res=read(sockfd,closebuf,sizeof(closebuf));
	   		cout<<"~~"<<res<<"~~"<<endl;
	   		if(!res)
	   			break;
	   }
   close(sockfd);
}

void clientBoot(){
	myip=getMyIp();
	addServer();
	for(int i=0;i<10;i++)
		connlist[i]=-2;
}

void serverBoot(){
	myip=getMyIp();
	for(int i=0;i<10;i++)
		connlist[i]=-2;
}

int main ( int argc, char *argv[]){
	if(argc!=arg_no ){
		printf("%s\n","Arguments missing!");
		return 0;
	}
	else{
		role=*argv[1];
		//port=*argv[2];
		strcpy(port, argv[2]);

		//printf("Port number is %s\n",port );
		
		if(role=='c'){
			clientBoot();
			listener();
			//printf("Its a client\n");
		}
		else if(role=='s'){
			serverBoot();
			listener();
			//printf("Its a server\n");
		}
		else{
			//printf("Unknown role\n");
			return 0;
		}
	}
	
	return 0;
}