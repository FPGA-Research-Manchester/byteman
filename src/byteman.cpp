#include <iostream>
#include "byteman.h"
using namespace std;


int main(){
	cout << x << endl;
	x = 0xA5A5A5A5;
	cout << x << endl;
	cout << "byteman version "<< VERSION << "(Build "<<VERSION_BUILD<<")" << endl;
	
	return 0;
}