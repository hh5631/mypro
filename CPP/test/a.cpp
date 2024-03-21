#include<iostream>
#include<fstream>
#include <vector>
#include <list>

using namespace std;


int main(){
        list<int> v = {1, 2, 3, 4, 5};
        v.push_back(6);
        v.pop_front();
        for(auto i = v.begin(); i != v.end(); i++){
                if (*i==3)
                {
                       *i=100;
                }
                
                cout << *i << endl;
        }
        

        return 0;

}