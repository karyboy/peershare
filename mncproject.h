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

struct cnxn{
	int id;
	char *ipaddr;
	char *port;
	struct cnxn *next;
};

typedef struct cnxn list;
list *cnxnroot;

list *addConnection(char *ipaddr,char *port){
	printf("%s++%s\n", ipaddr,port);
	list *tmp=cnxnroot;
	while(tmp->next!=0){
		tmp=tmp->next;
	}
	list *add=(list *)malloc(sizeof(list));
	add->id=tmp->id+1;
	add->ipaddr=ipaddr;
	add->port=port;
	add->next=0;
	tmp->next=add;
	return add;
}

void initializeList(){
	cnxnroot=(list *)malloc(sizeof(list));
	cnxnroot->id=0;
	cnxnroot->ipaddr="0.0.0.0";
	cnxnroot->port="0";
	cnxnroot->next=0;
}

list *deleteConnection(int id){
	list *tmp=cnxnroot;
	list *prev=cnxnroot;
	list *remd;
	while(tmp!=0){
		if(tmp->id==id && tmp->id!=0){
			//printf("deleted %s\n",tmp->ipaddr );
			remd=tmp;
			prev->next=tmp->next;
			break;
		}
		prev=tmp;
		tmp=tmp->next;
	}
	return remd;
}

char *formString(){
	list *tmp=cnxnroot;
	char *str=(char *)malloc(0);
	while(tmp!=0){
		if(tmp->id!=0){
			 strcat(str,tmp->ipaddr);
			 strcat(str, "~");
			 strcat(str,tmp->port);
			 strcat(str,"|");
		}
		tmp=tmp->next;
	}
	return str;
}

void formList(char *str){
	char *ptr;
	char arr[10][20];
	int i=0;
	ptr=strtok(str,"|");
	while(ptr!=NULL){
		strcpy(arr[i], ptr);
		//printf("===%s\n",arr[i]);
		i++;
		ptr=strtok(NULL,"|");
	}

	for(int j=0;j<i;j++){
		char *ipaddr,*port;
		//printf("--%s\n", arr[j]);
		ipaddr=strtok(arr[j],"~");
		port=strtok(NULL,"~");
		addConnection((char *)ipaddr, (char *)port);
		//printf("%s--%s--%d\n", ipaddr,port,j);
	}
}

void traverseList(){
	list *tmp=cnxnroot;
	while(tmp!=0){
		printf("%s--%s\n",tmp->ipaddr,tmp->port);
		tmp=tmp->next;
	}
}





#endif