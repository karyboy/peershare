
#include "mncproject.h"

int main(){
	string str="./a.out s 8888";
	vector<string> v=tokenize2(str, " ");
	for(int i=0;i<v.size();i++){
		cout<<v.at(i)<<endl;
	}
	cout<<v.size();
}