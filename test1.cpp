
#include "mncproject.h"

int main(){
	list *tmp;
	initializeList();
	addConnection("192.168.1.2","1224");
	addConnection("192.100.100.100", "8000");
	//traverseList();
	//tmp=deleteConnection(2);
	//traverseList();
	//printf("removed %d-%s\n",tmp->id,tmp->ipaddr);
	addConnection("159.168.123.1", "9090");
	//traverseList();
	//tmp=deleteConnection(1);
	//traverseList();
	//printf("removed %d-%s\n",tmp->id,tmp->ipaddr);
	 char *str=formString();
	 printf("%s\n", str);
	 formList(str);
	traverseList();
	return 0;
}