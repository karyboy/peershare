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

using namespace std;

vector < vector<string> > connections;

vector<string> addConnection(string ipaddr,string port){
	vector<string> conn(3);
	conn[1]=ipaddr;
	conn[2]=port;
	int id=connections.size();
	char ids[10];
	sprintf(ids, "%d", id);
	conn[0]=string(ids);
	connections.push_back(conn);
	return conn;
}

vector<string> removeConnection(string id){
	for(int i=0;i<connections.size();i++){
		vector<string> tmp=connections[i];
		if(tmp[0]==id){
			//cout<<"found connection";
			connections.erase(connections.begin()+i);
		}
	}
}

void emptyConnections(){
	connections.clear();
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
    cout<<"{===="<<pipaddr<<"}";
    return pipaddr;
}

void traverseConnections(){
	for(int i=0;i<connections.size();i++){
		vector<string> tmp=connections[i];
		printf("details are === %s--%s--%s\n",tmp[0].c_str(),tmp[1].c_str(),tmp[2].c_str() );
	}
}

string formPeerString(){
	string str="peers_";
	for(int i=0;i<connections.size();i++){
		vector<string> tmp=connections[i];
		string tmpstr=tmp[1]+"|"+tmp[2];
		str=str+"["+tmpstr+"]";
	}
	return str;
}

void formPeerVector(string str){
	int pos=str.find_first_of("[");
	while(pos!=string::npos){
		int pos1=str.find_first_of("]",pos+1);
		string tmp=str.substr(pos+1,pos1-pos-1);
		pos=str.find_first_of("[",pos1+1);
		int subpos=tmp.find_first_of("|");
		string ip=tmp.substr(0,subpos);
		string port=tmp.substr(subpos+1,str.length());
		//cout<<ip<<"-"<<port<<"\n";
		addConnection(ip, port);
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



#endif