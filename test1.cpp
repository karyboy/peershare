
#include "mncproject.h"

int main(){
	string str="[192.168.1.4|9999][192.168.1.4|7777]";
	addServer();
	formPeerVector(str);
	for(int i=0;i<connections.size();i++){
		connections[i][2]="000";
		//cout<<connections[i][2]<<endl;
	}
	traverseConnections();
}