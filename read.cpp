#include<iostream>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

#define FILE_NAME "/tmp/keyboard_event_fifo"


int main(){
         int fd =open(FILE_NAME,O_RDONLY|O_NONBLOCK);
        if(fd<0){
                perror("open");
                return 1;
        }
        int res;
        int buffer[2];
        if (fd!=-1)
        {     
                  while (true)
        {
        res =read(fd,buffer,sizeof(buffer));
                        cout<<"----"<<buffer[0]<<"----"<<buffer[1]<<endl;      
                          }
        
                
        }
        
        char key;


        return 0;
}