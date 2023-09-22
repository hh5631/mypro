#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <list>
#include <filesystem>
#include <unordered_map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
using namespace std;


//TODO:自定义函数（需要加namespace Utils:前缀）
namespace Utils
{
        list<string> split(string str,string splitstr);
        string getFileAllData(std::ifstream& file);
        string rmspc(string str);




} // namespace Utils


//TODO:INI配置文件解析类
class IniParser {
public:
    IniParser();
    explicit IniParser(const std::string& filename);

    void Load(const std::string& filename);
    std::string GetValue(const std::string& section, const std::string& key) const;

private:
    void Trim(std::string& str);

    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;
};



//TODO:Socket通信server类
class Server {
public:
    Server(int port);
    void Listen(int client_num);
    int Accept();
    string receive();
    void send(const std::string& message);

public:
    int serverSocket;
    int clientSocket;
    sockaddr_in serverAddress{};
};



//TODO:Socket通信client类
class Client {
public:
    Client(const std::string& serverIP, int port);

    void send(const std::string& message);

    string receive();

public:
    int clientSocket;
    struct sockaddr_in serverAddress{};
};
