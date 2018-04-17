#include <iostream>
#include <unistd.h>
#include "Timer.h"
#include "Transcoder.h"
#include "RPCServer.h"
#include <grpc++/grpc++.h>
#include <thread>
#include <vector>
#include <fstream>
using namespace Noovo;
using namespace std;
char ch;


void RunServer() {
  std::string server_address("0.0.0.0:50053");
  ServiceImpl service;

  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<grpc::Server> server(builder.grpc::ServerBuilder::BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->grpc::Server::Wait();
}


static void test2(){

}

int main(){

    RunServer();
    
    return 0;

}