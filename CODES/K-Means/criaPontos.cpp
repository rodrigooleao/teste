#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main(){

	int n, a, b;
	srand( time(NULL));

	cin >>n;

	for( int i = 0 ; i < n ; i++){
		a = (rand() % 1000) - 500;
		b = (rand() % 1000) - 500;

		cout <<a<<" "<<b<<endl;
	}
}
