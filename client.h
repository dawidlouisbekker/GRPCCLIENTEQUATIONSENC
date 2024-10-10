#pragma once
#ifndef THISGRPCLIENT
#define THISGRPCCLIENT

#include <iostream>
#include <memory>
#include <string>
#include <random>
#include <Windows.h>
#include <windows.h>
#include <stdlib.h>
#include <grpcpp/grpcpp.h>
#include "equations.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ClientReaderWriter;
using validator::Validator;
using validator::IP;
using validator::Answer;
using validator::Variables;
using validator::Port;
using validator::Recieved;

std::string clientPort;
class ValidatorClient {
private:
    double answer;
public:

    ValidatorClient(std::shared_ptr<Channel> channel) : stub_(Validator::NewStub(channel)) {
        answer = 0;
    }

    void EstablishConnection(const std::string& ip) {
        IP request;
        request.set_ip(ip);
        Variables reply;
        ClientContext context;
        Answer ans;

        Status status = stub_->EstablishCon(&context, request, &reply);

        if (status.ok()) {
           /* std::cout << "Received: a=" << reply.a()
                << ", b=" << reply.b()
                << ", c=" << reply.c()
                << ", x=" << reply.x()
                << " Equation context=" << reply.equtioncontext() << std::endl;*/
            
            Equations calc = Equations();
            
            answer = calc.CalcAnswer(reply.a(), reply.b(), reply.c(), reply.x(), reply.equtioncontext());

            std::cout << "Almost there..." << std::endl;
            return;


        }
        else {


            return;
           /* std::cerr << "RPC failed: " << status.error_code()
                << ": " << status.error_message() << std::endl;*/
        }
    }

    void SendAnswer() {
        Answer request;
        request.set_answer(answer);
        Variables reply;
        ClientContext context;

        Status status = stub_->GiveVariables(&context, request, &reply);

        if (status.ok()) {
                Equations calc = Equations();

                answer = calc.CalcAnswer(reply.a(), reply.b(), reply.c(), reply.x(), reply.equtioncontext());
                std::cout << "Connected" << std::endl;
                return;
        }
        else {
            std::cerr << "RPC failed: " << status.error_code()
                << ": " << status.error_message() << std::endl;
            return;
        }
    }

    void NewPort() {
        ClientContext context;
        std::shared_ptr<ClientReaderWriter<Port, Recieved>> stream(
            stub_->NewPort(&context));

        std::thread writer([stream]() {
            for (int i = 0; i < 5; ++i) {
                Port port;
                port.set_port(5000 + i);  // Sending different port numbers
                std::cout << "Sending port: " << port.port() << std::endl;
                stream->Write(port);
            }
            stream->WritesDone();
            });

        Recieved response;
        while (stream->Read(&response)) {
            std::cout << "Received ans: " << response.ans() << std::endl;
        }

        writer.join();
        Status status = stream->Finish();
        if (!status.ok()) {
            std::cout << "NewPort rpc failed." << std::endl;
        }
    }

private:
    std::unique_ptr<Validator::Stub> stub_;
};

void WebHookclient(const std::string url) {
    // Open the URL in the default web browser
    std::system("start http://127.0.0.1:8080/adbfinejwnileubfjksn");

}

int client(std::string port) {
        std::string target_str = "127.0.0.1:" + port;
        std::string input;
        //std::chrono::microseconds(500);
        bool isFInished = false;
        ValidatorClient client(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
        client.EstablishConnection(target_str);
        //std::cout << std::endl << "client started" << std::endl << std::endl;

        while (isFInished == false) {
            std::cout << "Established Connection";
            client.SendAnswer();
            std::cout << std::endl << "again(y/n):";
            std::getline(std::cin, input);
            if (input == "y") {
                continue;
            }
            else {
                isFInished = true;
            }

        }
    return 0;
}



#endif // !"THISGRPCLIENT"

