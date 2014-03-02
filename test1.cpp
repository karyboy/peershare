
#include "mncproject.h"

int main(){
	string str="download 21 a.txt 3 b.txt 4 c.txt";
	vector<string> v=tokenize2(str, " ");
	for(int i=0;i<v.size();i++){
		cout<<v.at(i)<<endl;
	}
	cout<<v.size();
}