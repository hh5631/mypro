#include"utils.h"

list<string> Utils::split(string str,string splitstr){

   std::list<std::string> result;

    std::size_t startPos = 0;
    std::size_t foundPos;

    while ((foundPos = str.find(splitstr, startPos)) != std::string::npos) {
        std::string part = str.substr(startPos, foundPos - startPos);
        result.push_back(part);
        startPos = foundPos + splitstr.length();
    }

    // 处理最后一个分隔符后的部分或整个字符串本身
    std::string lastPart = str.substr(startPos);
    result.push_back(lastPart);

    return result;
}

string Utils::getFileAllData(std::ifstream& file){
   if (!file.is_open()) {
        std::cerr << "Error: File is not open." << std::endl;
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

string Utils::rmspc(string str)
{   
    string result;
    // 遍历字符串，将非空格字符拼接到结果字符串中
    for (char c : str) {
        if (c != ' ') {
            result += c;
        }
    }
        return result;
}

//INI解析类
IniParser::IniParser() {}

IniParser::IniParser(const std::string& filename) {
    Load(filename);
}

void IniParser::Load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string currentSection;
    while (std::getline(file, line)) {
        Trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;  // Skip empty lines and comments
        } else if (line[0] == '[' && line[line.length() - 1] == ']') {
            // Section header
            currentSection = line.substr(1, line.length() - 2);
        } else {
            // Key-value pair
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                Trim(key);
                Trim(value);
                if (!currentSection.empty()) {
                    data[currentSection][key] = value;
                }
            }
        }
    }
    file.close();
}
std::string IniParser::GetValue(const std::string& section, const std::string& key) const {
    auto sectionIter = data.find(section);
    if (sectionIter != data.end()) {
        const auto& sectionData = sectionIter->second;
        auto keyIter = sectionData.find(key);
        if (keyIter != sectionData.end()) {
            return keyIter->second;
        }
    }
    return "";
}
void IniParser::Trim(std::string& str) {
    const std::string whitespace = " \t\r";
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos) {
        str = "";
        return;
    }
    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;
    str = str.substr(strBegin, strRange);
}



Server::Server(int port) : serverSocket(0), clientSocket(0) {
        if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }
        
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        
        if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
            perror("Binding failed");
            exit(EXIT_FAILURE);
        }
    }
    void Server::Listen(int client_num){    
            if (listen(serverSocket, client_num) <0) {
                perror("Listening failed");
                 exit(EXIT_FAILURE);
            }
        
        std::cout << "Server is listening on port " << std::endl;
    }
    int Server::Accept(){
        sockaddr_in clientAddress{};
        socklen_t clientAddrLength = sizeof(clientAddress);
         clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, (socklen_t*)&clientAddrLength);
        if (clientSocket == -1) {
            perror("Accepting failed");
            exit(EXIT_FAILURE);
        }
        
        cout << "Client connected" << std::endl;
        return clientSocket;
    }
    
    string Server::receive() {
   const int bufferSize = 4096;  // 设置接收缓冲区大小
    char buffer[bufferSize];
    string receivedData;

    ssize_t bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, bufferSize, 0)) > 0) {
        receivedData.append(buffer, bytesRead);
    }

    if (bytesRead == -1) {
        perror("Receive failed");
    }

    return receivedData;
    }
    
    void Server::send(const std::string& message) {
    const char* data = message.c_str();
    size_t totalBytesSent = 0;
    size_t messageSize = message.size();

    while (totalBytesSent < messageSize) {
        ssize_t bytesSent = ::send(clientSocket, data + totalBytesSent, messageSize - totalBytesSent, 0);
        if (bytesSent == -1) {
            perror("Send failed");
            break;
        }
        totalBytesSent += bytesSent;
    }
    }




     Client::Client(const std::string& serverIP, int port) : clientSocket(0) {
        if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket creation failed");
        }
        
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        
        if (inet_pton(AF_INET, serverIP.c_str(), &(serverAddress.sin_addr)) <= 0) {
            perror("Invalid address/ Address not supported");
        }
        
        if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
            perror("Connection failed");
            exit(EXIT_FAILURE);
        }
    }
    
    void Client::send(const std::string& message) {
     const char* data = message.c_str();
    size_t totalBytesSent = 0;
    size_t messageSize = message.size();

    while (totalBytesSent < messageSize) {
        ssize_t bytesSent = ::send(clientSocket, data + totalBytesSent, messageSize - totalBytesSent, 0);
        if (bytesSent == -1) {
            perror("Send failed");
            break;
        }
        totalBytesSent += bytesSent;
    }
    }
    
    string Client::receive() {
    const int bufferSize = 4096;  // 设置接收缓冲区大小
    char buffer[bufferSize];
    string receivedData;

    ssize_t bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, bufferSize, 0)) > 0) {
        receivedData.append(buffer, bytesRead);
    }

    if (bytesRead == -1) {
        perror("Receive failed");
    }

    return receivedData;
    }