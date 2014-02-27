
#include "mncproject.h"

int main(){
	string str="[192.168.1.4|9999][192.168.1.4|7777]";
	addServer();
	formPeerVector(str);
	bool check=checkConnection("192.168.1.4", "888");
	cout<<check;
	//traverseConnections();
}