
#include "mncproject.h"

int main(){
	string str="[192.168.1.4|9999][192.168.1.4|7777]";
	addServer();
	formPeerVector(str);
	//traverseConnections();
}