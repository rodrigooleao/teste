#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<string> Parse( string str , char token){
    vector<string> result;
    string r;

    int ini = 0, fim = 0;

    while( fim < str.size() ){
        if( str[fim] == token ){
            r = str.substr( ini , (fim - ini));
            
            cout<<r<<endl;
            result.push_back( r );

            while( str[fim] == token)fim++;
            ini = fim;
        }
        fim++;
    }

    r = str.substr( ini , (fim - ini));
    cout<<r<<endl;
    result.push_back( r );

    return result;
}
int main(){
    
    
    string str = "Rodrigo Otavio    rodrigues";

    Parse( str , ' ');

    
}