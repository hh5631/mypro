#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "utils.h"
#define BUFFER_SIZE 1024

std::string convertToUTF8(const std::string& str) {
    // 在这里实现将文件名转换为UTF-8编码的逻辑
    // ...
    return str;
}

int main() {
    string filePath = "/home/hh/IDEAPRO/tris/target/TRIS_SERVER.war";  std::string fileName = "TRIS_SERVER.war";
   
    //string filePath = "/home/hh/IDEAPRO/gw/target/GW_server.war"; 
   // string filePath = "/tmp/TRIS_SERVER.war";
    //string fileName = "GW_server.war";
    

    // 打开文件
     std::ifstream file(filePath, std::ios::binary);
    // if (!file.is_open()) {
    //     std::cerr << "Failed to open file: " << filePath << std::endl;
    //     return 1;
    // }

    Client client("192.168.100.178",12346);


    // // 发送文件名长度
    // size_t fileNameLength = fileName.size();
    // if (send(sock, &fileNameLength, sizeof(fileNameLength), 0) < 0) {
    //     std::cerr << "Failed to send file name length" << std::endl;
    //     return 1;
    // }

    // 发送文件名
   // thread t1([&client,fileName](){client.send(fileName);});

    string buffer = Utils::getFileAllData(file) ;
    // // 发送文件内容.
    //cout<<buffer.length()<<endl;
    //client.send(filePath);
    client.send(fileName+"@#@#@#"+buffer);
  // thread t2([&client,fileName,buffer](){});
//    thread t2([&client,filePath,buffer](){client.send(filePath);});
//     t2.join();
    //client.send(fileName+"@#@"+buffer);
    //client.send(buffer);
    // 关闭连接和文件
    // file.close();

    // std::cout << "File sent successfully" << std::endl;


}   
