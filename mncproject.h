// Author - Karnesh Mehra
// karneshm@buffalo.edu

#ifndef mncproject
#define mncproject

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/select.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <sys/time.h>

using namespace std;

char port[10];
char role;
string myip;
string serverip;
string serverport;
int connlist[10];
struct addrinfo ai;
struct sockaddr_storage conns;
struct addrinfo *res;
int listenfd;
int maxsock;
string test;
fd_set fdreads,fdmain,fdwrites;
socklen_t conn_size;

char buf[100];
int commandbuffer=100;
int filebuffer=3000;

vector < vector<string> > connections;

vector < vector<string> > connectd;

int handleNewConnection();
void redoFDSET();
bool getData(int);
void permData(int);
void registerWithServer(string,string);
void assignMaxFD();
void sendMsg(string,string,string);
void sendCnxnList();
void connectTo(string,string,string);
void addPermanent(string,string,int);
void addToConnList(int);
void sendTo(string,string);
void terminate(string);
bool sendFile(string,string,string);
void fileData(int,string);
void pushUpload(string,string);
void requestDownload(string,string);
void handleExit();
//

string getDomainName(string url){
	struct hostent *he;
	struct in_addr ipv4addr;
	inet_pton(AF_INET, url.c_str(), &ipv4addr);
	he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
	if(he==NULL)
		return "NA";
	else
		return string(he->h_name);
}

int strToInt(string str){
	return atoi(str.c_str());
}

string getFileName(string fullpath){
	int i=fullpath.rfind("/");
	return fullpath.substr(i+1);
}

vector<string> addConnection(string ipaddr,string port){
	vector<string> conn(4);
	conn[1]=ipaddr;
	conn[2]=port;
	int id=connections.size()+1;
	char ids[10];
	sprintf(ids, "%d", id);
	conn[0]=string(ids);
	conn[3]=getDomainName(ipaddr);
	connections.push_back(conn);
	return conn;
}

void reConnection(int rid){
	for(int i=0;i<connections.size();i++){
		//cout<<"-->"<<connectd[i][0]<<endl;
		char ids[10];
		if(strToInt(connections[i][0])>rid){
			int newid=strToInt(connections[i][0])-1;
			sprintf(ids, "%d", newid);
			connections[i][0]= string(ids);
		}
		
	}
}

void removeConnection(string id){
	string rid;
	for(int i=0;i<connections.size();i++){
		vector<string> tmp=connections[i];
		if(tmp[0]==id ){
			rid=tmp[0];
			//cout<<"removing from server "<<endl;
			connections.erase(connections.begin()+i);
		}
	}
	reConnection(strToInt(rid));
}

void emptyConnections(){
	connections.erase(connections.begin()+1,connections.end());
	//connections.clear();
}
void updatePort(string id,string port){
	for(int i=0;i<connections.size();i++){
		vector<string> tmp=connections[i];
		if(tmp[0]==id){
			//cout<<"found connection";
			connections[i][2]=port;
		}
	}
}

string getIpPeer(int fd){
	struct sockaddr_storage peername;
	socklen_t plen=sizeof(peername);
	char pipaddr[INET6_ADDRSTRLEN];
	getpeername(fd, (struct sockaddr *)&peername, &plen);
	struct sockaddr_in *s = (struct sockaddr_in *)&peername;
    //port = ntohs(s->sin_port);
    inet_ntop(AF_INET, &s->sin_addr, pipaddr, sizeof pipaddr); 
    //cout<<"{===="<<pipaddr<<"}";
    return pipaddr;
}

string getIpMy(int fd){
	struct sockaddr_storage peername;
	socklen_t plen=sizeof(peername);
	char pipaddr[INET6_ADDRSTRLEN];
	getsockname(fd, (struct sockaddr *)&peername, &plen);
	struct sockaddr_in *s = (struct sockaddr_in *)&peername;
    //port = ntohs(s->sin_port);
    inet_ntop(AF_INET, &s->sin_addr, pipaddr, sizeof pipaddr); 
    //cout<<fd<<"{===="<<pipaddr<<"}";
    return pipaddr;
}

string getMyIp() 
{
    char buffer[INET_ADDRSTRLEN];
    string ipaddr="8.8.8.8";
    string porta="53";
    sockaddr_in name;
    socklen_t namelen = sizeof(name);
    struct sockaddr_in servaddr;
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    int optval=1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(ipaddr.c_str());
    servaddr.sin_port=htons(atoi(porta.c_str()));
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    getsockname(sockfd, (sockaddr*) &name, &namelen);
    inet_ntop(AF_INET, &name.sin_addr, buffer, INET_ADDRSTRLEN);
    //printf("_______%s\n",buffer );
    close(sockfd);
    return string(buffer);
}

void traverseConnections(){
	//cout<<"Server List"<<endl;
	for(int i=0;i<connections.size();i++){
		vector<string> tmp=connections[i];
		printf("%s|%s|%s|%s\n",tmp[0].c_str(),tmp[1].c_str(),tmp[2].c_str(),tmp[3].c_str() );
	}
}

string formPeerString(){
	string str="peers_";
	for(int i=0;i<connections.size();i++){
		vector<string> tmp=connections[i];
		if(tmp[0]!="1" || role=='s'){							// if not server row
			string tmpstr=tmp[1]+"|"+tmp[2];
			str=str+"["+tmpstr+"]";
		}
	}
	return str;
}

void formPeerVector(string str){
	int pos=str.find_first_of("[");
	//cout<<"\nThis is it"<<pos<<endl;
	while(pos!=string::npos){
		//cout<<pos<<endl;
		int pos1=str.find_first_of("]",pos+1);
		string tmp=str.substr(pos+1,pos1-pos-1);
		pos=str.find_first_of("[",pos1+1);
		int subpos=tmp.find_first_of("|");
		string ip=tmp.substr(0,subpos);
		string port=tmp.substr(subpos+1,str.length());
		if(port!=string(::port) || ip!=myip){    ///  compare ip as well on student servers {ip!=myip && } 
			addConnection(ip, port);
		}
	}
}

std::vector<std::string> tokenize(std::string str,std::string delim){
	std::size_t f1 = str.find_first_of(delim);
	if(f1!=std::string::npos){
		std::size_t f2 = str.find_first_of(delim,f1+1);
		//std::cout<<"herenow"<<f2;
		std::vector<std::string> vect(3);
		std::string cmd= str.substr(0,f1);
		std::string o1= str.substr(f1+1,f2-f1-1);
		std::string o2= str.substr(f2+1,str.length()-f2);
		vect[0]=cmd;
		vect[1]=o1;
		vect[2]=o2;
		return vect;
	}
	else{
		std::vector<std::string> vect(1);
		vect[0]=str;
		return vect;
	}
}

std::vector<std::string> tokenize2(std::string str,std::string delim){
	size_t f1=0,f2=0;
	vector<std::string> toks(0);
	while((f1=str.find_first_of(delim,f1+1))!=string::npos){
			string o=str.substr(f2,f1-f2);
			toks.push_back(o);
			f2=f1+1;
	}
	string o=str.substr(f2,str.length()-f2);
	toks.push_back(o);
	return toks;
}

bool checkConnection(string ip,string port){
	for(int i=0;i<connections.size();i++){
		vector<string> tmp=connections[i];
		if((tmp[1]==ip && tmp[2]==port) || (tmp[3]==ip && tmp[2]==port) ){
			//cout<<"this is in"<<endl;
			return true;
		}
	}
	return false;
}

void tester (){
	//cout<<getDomainName("8.8.8.8");
}

void addConnectd(string ipaddr,string port,int fd){
	vector<string> conn(5);
	conn[1]=ipaddr;
	conn[2]=port;
	stringstream ss;
	ss << fd;
	string ss1=ss.str();
	conn[3]=ss1;
	int id=connectd.size()+1;
	char ids[10];
	sprintf(ids, "%d", id);
	conn[0]=string(ids);
	conn[4]=getDomainName(ipaddr);
	connectd.push_back(conn);
}

void addServer(){
	addConnection(serverip,serverport);
	addConnectd(serverip,serverport,-1);
}

void traverseConnectd(){
	//cout<<"Open Connections"<<endl;
	for(int i=0;i<connectd.size();i++){
		vector<string> tmp=connectd[i];
		printf("%s|%s|%s|%s\n",tmp[0].c_str(),tmp[1].c_str(),tmp[2].c_str(),tmp[4].c_str() );
	}
}

void closeAllConnected(){
	for(int i=1;i<connectd.size();i++){
		vector<string> tmp=connectd[i];
		//cout<<"-->"<<tmp[0]<<"->"<<tmp[2]<<endl;
		terminate(tmp[0]);
	}
}

bool checkFd(int fd){
	//cout<<"\nFD to Check "<<fd<<endl;
	stringstream ss;
	ss << fd;
	string ss1=ss.str();
	for(int i=0;i<connectd.size();i++){
		vector<string> tmp=connectd[i];
		if(ss1==tmp[3]){
			return true;
		}
		//printf("%s|%s|%s|%s\n",tmp[0].c_str(),tmp[1].c_str(),tmp[2].c_str(),tmp[3].c_str() );
	}
	return false;
}

bool checkConnectd(string ip,string port){
	//cout<<"\nFD to Check by string "<<endl;
	for(int i=0;i<connectd.size();i++){
		vector<string> tmp=connectd[i];
		if((ip==tmp[1] && port==tmp[2]) || (ip==tmp[4] && port==tmp[2])){
			return true;
		}
		//printf("%s|%s|%s|%s\n",tmp[0].c_str(),tmp[1].c_str(),tmp[2].c_str(),tmp[3].c_str() );
	}
	return false;
}

vector<string> getFd(string id){
	vector<string> tmp;
	//cout<<"\ngetting fd "<<connectd.size()<<endl;
	for(int i=0;i<connectd.size();i++){
		vector<string> tmpa=connectd[i];
		if(id==tmpa[0]){
			//cout<<"YOYo in id"<<endl;
			tmp=connectd[i];
		}
			
		//printf("%s|%s|%s|%s\n",tmp[0].c_str(),tmp[1].c_str(),tmp[2].c_str(),tmp[3].c_str() );
	}
	return tmp;
}

vector<string> getFd(string ip,string port){
	vector<string> tmp;
	//cout<<"\ngetting fd by ip "<<endl;
	for(int i=0;i<connectd.size();i++){
		vector<string> tmpa=connectd[i];
		if(ip==tmpa[1] && port==tmpa[2])
			tmp=connectd[i];
		//printf("%s|%s|%s|%s\n",tmp[0].c_str(),tmp[1].c_str(),tmp[2].c_str(),tmp[3].c_str() );
	}
	return tmp;
}

vector<string> getConnection(string ip,string port){
	vector<string> tmp;
	//cout<<"\ngetting conn by ip "<<endl;
	for(int i=0;i<connections.size();i++){
		vector<string> tmpa=connections[i];
		if(ip==tmpa[1] && port==tmpa[2])
			tmp=connections[i];
		//printf("%s|%s|%s|%s\n",tmp[0].c_str(),tmp[1].c_str(),tmp[2].c_str(),tmp[3].c_str() );
	}
	return tmp;
}

void reConnectd(int rid){
	for(int i=1;i<connectd.size();i++){
		//cout<<"-->"<<connectd[i][0]<<endl;
		char ids[10];
		if(strToInt(connectd[i][0])>rid){
			int newid=strToInt(connectd[i][0])-1;
			sprintf(ids, "%d", newid);
			connectd[i][0]= string(ids);
		}
		
	}
}

void removeConnectd(string id){
	string rid;
	for(int i=0;i<connectd.size();i++){
		vector<string> tmp=connectd[i];
		if(tmp[0]==id){
			rid=tmp[0];
			cout<<"Removing connection with id "<<id<<endl;
			connectd.erase(connectd.begin()+i);
		}
	}
	reConnectd(strToInt(rid));
}



#endif