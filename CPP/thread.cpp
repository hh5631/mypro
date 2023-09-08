#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h> 
#include <termios.h>
#include <thread>
#include <future>

using namespace std;

#define FILE_NAME "/tmp/keyboard_event_fifo"


int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int getch() {
    int ch;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

int main(){


        if (access(FILE_NAME,F_OK)==0)
        {
           cout<<"File is Exists"<<endl;
        }
        else{
              if(mkfifo(FILE_NAME,0666)<0){
                perror("mkfifo");
                return 1;
              }
        }
        int fd =open(FILE_NAME,O_WRONLY);
        if(fd<0){
                cout<<"open failed"<<endl;
               // perror("open");
                return 1;
        }
        else{
             cout<<"open successful"<<endl;
        }
        int buffer[2];
        int key;
        while (true)
        {   
                if(kbhit()){
                        key=getch();
                        cout<<"Press key :"<<key<<endl;
                        buffer[0]=1;
                        buffer[1]=key;
                        write(fd,buffer,sizeof(buffer));
                            if (!kbhit())
                                {
                                        cout<<"Release key :"<<key<<endl;
                                        buffer[0]=0;
                                        buffer[1]=key;
                                        write(fd,buffer,sizeof(buffer));
                                }     
               
                                  
                }
               
        }

            
        return 0;
}