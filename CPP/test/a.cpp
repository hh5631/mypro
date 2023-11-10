#include<iostream>
#include<thread>
#include <string>
using namespace std;

class Person
{
private:
        /* data */
public:
        Person(/* args */);
        ~Person();
        template <class T>
        T Get();
        string name;
        int age;
};

Person::Person()
{
}

Person::~Person()
{
}

template <class T>
T Person::Get()
{
        return this->name;
}

int main(){
        // thread t([=]{
        //         for (int i = 0; i< 1000000; i++)
        //        {
        //                 cout<<i<<endl;
        //         }
        // });
        // t.join();
        Person p;
        p.name = "aaa";
        cout<< p.Get<string>()<<endl;
        return 0;
}