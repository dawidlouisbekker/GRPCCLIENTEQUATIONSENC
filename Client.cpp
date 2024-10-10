#include <thread>
#include <string>
#include <grpcpp/grpcpp.h>
#include "messages.grpc.pb.h"
#include "messages.pb.h"
#include "client.h"
#include "equations.h"
#include "apiclient.h"
#include <google/protobuf/message.h>
#include <iostream>
#include <string>
#include <chrono>



int main() {
	std::string input;
	std::chrono::seconds(3);
	int grpcPort = 0;
	int NumTries = 0;
	bool Close = false;

	do {
		grpcPort = CallHttpServer();
	} while (grpcPort <= 0);
	std::chrono::seconds(2);
	client(std::to_string(grpcPort));
	std::cin >> input;


	//grpcPort = CallHttpServer();
	//std::cout << "port:";
	//std::cin >> grpcPort;
/*	while ((client(std::to_string(grpcPort)) == 0) && NumTries < 5) {
		CallHttpServer(grpcPort);
		std::cout << "connectint to port: " << grpcPort;
		NumTries++;
	}
	std::getline(std::cin, input);*/



	//CallHttpServer();
	//std::cout << " done ";


}