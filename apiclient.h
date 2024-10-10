#ifndef APICLIENT
#define APICLIENT

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <sstream>
#include "equations.h"
#include "client.h"
 
#pragma comment(lib, "Ws2_32.lib")


// Function to convert char to binary
std::string charToBinary(char c) {
    std::string binary;
    for (int i = 7; i >= 0; --i) {
        binary += ((c >> i) & 1) ? '1' : '0';
    }
    return binary;
}

// Function to convert string to binary
std::string stringToBinary(const std::string& s) {
    std::string binary;
    for (char c : s) {
        binary += charToBinary(c) + " ";
    }
    return binary;
}

void ExtractValues(int& A, int& B, int& C,int& X , std::string& EquationContext, std::string response){

    std::istringstream responseStream(response);
    std::string line;
    // Read response lines
    while (std::getline(responseStream, line)) {
        if (line.find("A: ") == 0) {
            A = stoi(line.substr(3)); // Extract value after "A: "
        }
        else if (line.find("B: ") == 0) {
            B = stoi(line.substr(3)); // Extract value after "B: "
        }
        else if (line.find("C: ") == 0) {
            C = stoi(line.substr(3)); // Extract value after "C: "
        }
        else if (line.find("X: ") == 0) {
            X = stoi(line.substr(3)); // Extract value after "X: "
        }
        else if (line.find("E: ") == 0) {
            EquationContext = line.substr(3); // Extract value after "E: "
            //std::cout << EquationContext << std::endl;
            std::chrono::milliseconds(500);
            std::cout << "connected" << std::endl;
        }
    }
}

int CallHttpServer() {
    std::chrono::seconds(4);
    std::cout << "Connecting..." << std::endl;
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server;
    const char* server_ip = "127.0.0.1"; // Use localhost
    const int server_port = 8080;

    // Initialize Winsock
      if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 0;
      }
    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    // Set up the server structure
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server.sin_addr);
    // Connect to the server
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(sock);
        WSACleanup();
        return 2;
    }
    else {
        std::cout << "connected to socket" << std::endl;
    }
    std::cout << "sending" << std::endl;
    // Send an HTTP GET request
    const char* request = "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: keep-alive\r\n\r\n";
    send(sock, request, strlen(request), 0);
    std::cout << std::endl << "Sent Request" << std::endl;
    // Receive the response
    char buffer[4096];
    std::string response = "";
    int bytesReceived;
    while ((bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        if (bytesReceived <= 0) {
            // Connection has been closed by the peer
        //closesocket(sock);
            std::cout << std::endl << "Nothing";

        }
            buffer[bytesReceived] = '\0'; // Null-terminate the buffer
            response += buffer; // Append to response

            std::cout << response;
            std::cout << "At response" << std::endl;
            std::istringstream responseStream(response);
            std::string line;
            std::string equationContext = "";
            int a = 0, b = 0, c = 0, x = 0;
            // Read response lines
            ExtractValues(a, b, c, x, equationContext, response);

            double ansd = 0.0;
            std::string ans ="";
            std::string fileteredEquationContext = equationContext.substr(0, 2); //remoces /r
            Equations WebCalc = Equations();
            ansd = WebCalc.CalcAnswer(a, b, c, x, fileteredEquationContext);
            //std::cout << std::endl << ansd << std::endl;
            ans = std::to_string(ansd);
            //std::cout << std::endl << ans << std::endl;
            std::cout << std::endl << "answer:" << ans << std::endl;
            send(sock, ans.c_str(), ans.length(), 0);
            std::chrono::seconds(1);
            int grpcPort = ((int)ansd % 65535);
            closesocket(sock);
            WSACleanup();
            return grpcPort;
           // closesocket(sock);
    }
    if (bytesReceived <= 0) {
            // Connection has been closed by the peer
        //closesocket(sock);
        std::cout << std::endl << "Nothing";
    }
    else { // bytesReceived < 0
            std::cerr << "Receive failed." << std::endl; 
    }
   
  /*  char portBuffer[4096];
    bytesReceived = 0;
    response.clear();
    bytesReceived = recv(sock, portBuffer, sizeof(portBuffer) - 1, 0);
    if (bytesReceived > 0) {
        portBuffer[bytesReceived] = '\0'; // Null-terminate the buffer
        response += portBuffer; // Append to response
        // closesocket(sock);
    }
    else if (bytesReceived == 0) {
        // Connection has been closed by the peer
    //closesocket(sock);

    }
    else { // bytesReceived < 0
        std::cerr << "Receive failed." << std::endl;
    }

    ExtractValues(a, b, c, x, equationContext, request);

    std::cout << stringToBinary(equationContext) << std::endl;
    // Output the extracted values
    std::cout << "A: " << a << std::endl;
    std::cout << "B: " << b << std::endl;
    std::cout << "C: " << c << std::endl;
    std::cout << "X: " << x << std::endl;
    std::cout << equationContext << std::endl;
    ansd = 0.0;
    ans = "";
    ansd = WebCalc.CalcAnswer(a, b, c, x, fileteredEquationContext);
    //ansd = WebCalc.CalcAnswer(a, b, c, x, equationContext);
    std::cout << std::endl << ansd << std::endl;
    
    ans = std::to_string(grpcPort);

    send(sock, ans.c_str(), ans.length(), 0);
    */
    
    //std::cout << std::endl << "port: " << grpcPort << std::endl;
    //GRPCPort = grpcPort;

    closesocket(sock);
    WSACleanup();
    return 0;
}


#endif // !1


