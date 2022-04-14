#include <iostream>
#include "byteman.h"
using namespace std;


int main(){
	cout << x << endl;
	x = 0xA5A5A5A5;
	cout << x << endl;
	cout << "byteman ver "<< VERSION << endl;
	
	return 0;
}