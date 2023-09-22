#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include "utils.h"
using namespace std;
#define BUFFER_SIZE 1024

int main() {
    IniParser reader("/opt/serverConf.ini");

    //读取配置项
    int listenPort = 12345;
    string savePath = "/tmp/";  
    string targetServerIP = "";
    string targetServerUname = "";
    string targetServerPassWd = "";
    string targetServerPath = "";
    string targetServerScript = "";
    // 创建 Socket 绑定 port
    Server server(12346);
    // 监听连接
    while (true)
    {       
    server.Listen(1);

    // 接受连接
    server.Accept();

    // //接收文件名长度
    // string fileNameLength = server.receive();
    // cout<<fileNameLength<<endl;
    //接收文件名
    string result="";
    thread t([&]{result=server.receive();});
    t.join();
    auto res = Utils::split(result,"@#@#@#");
    string fileName = res.front();
    string fileData = res.back();
    string scpCommand="";
        int a = fileName.find("TRIS_SERVER.war");
     // 使用 scp 命令将文件发送到目标服务器
    if (fileName.find("TRIS_SERVER.war")!=-1){
        targetServerIP = reader.GetValue("tris","tris_ip");
        targetServerUname = reader.GetValue("tris","tris_username");
        targetServerPassWd = reader.GetValue("tris","tris_passwd");
        targetServerPath = reader.GetValue("tris","tris_path");
        targetServerScript = reader.GetValue("tris","tris_script");
    }
    if (fileName.find("GW_server.war")!=-1){
        targetServerIP = reader.GetValue("gw","gw_ip");
        targetServerUname = reader.GetValue("gw","gw_username");
        targetServerPassWd = reader.GetValue("gw","gw_passwd");
        targetServerPath = reader.GetValue("gw","gw_path");
        targetServerScript = reader.GetValue("gw","gw_script");
    }       
    if (fileName==fileData){
         std::string result;    
    // 遍历字符串，将非空格字符拼接到结果字符串中
    for (char c : fileName) {
        if (!std::isspace(c)) {
            result += c;
        }
    }
        scpCommand = "sshpass -p "+targetServerPassWd+" scp -r " +result +" "+targetServerUname+ "@" + targetServerIP + ":" + targetServerPath;
    }
    else{
        cout<<"接收文件:"+fileName<<endl;

        // 接收文件内容并保存
        string saveFilePath = savePath + fileName;
        std::ofstream file(saveFilePath);
        if (!file.good()) {
            std::cerr << "Failed to create file: " << saveFilePath << std::endl;
            close(server.clientSocket);
            file.close();
            continue;
        }
 
        file.write(fileData.c_str(),fileData.length());
        file.close();
        cout << "File received and saved: " << saveFilePath << std::endl;
        scpCommand = "sshpass -p "+targetServerPassWd+" scp -r " + saveFilePath + " " +targetServerUname+ "@" + targetServerIP + ":" + targetServerPath;
    }

        // 关闭连接和文件
        close(server.clientSocket);

        cout<<"向"<<targetServerIP<<"发送文件:"<<fileName<<endl;      
        int scpResult = system(scpCommand.c_str());
        if (scpResult != 0) {
            std::cerr << "Failed to send file to target server" << std::endl;
            continue;
         }

        cout << "File sent to target server successfully" << std::endl;

        // 在目标服务器上执行 shell 脚本
        cout<<"执行目标服务器脚本:"+targetServerScript<<endl;
        string scriptCommand = "sshpass -p "+targetServerPassWd+" ssh "+targetServerUname+ "@" + targetServerIP + " \"" + targetServerScript + "\"";
        int scriptResult = system(scriptCommand.c_str());
        if (scriptResult != 0) {
            cerr << "Failed to execute shell script on target server" << std::endl;
            continue;
        }

        cout << "Shell script executed on target server successfully" << std::endl;
    }
        return 0;
}