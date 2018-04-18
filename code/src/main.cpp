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

struct buffer_data {
    uint8_t *ptr;
    size_t size; ///< size left in the buffer
};
static int read_packet(void *opaque, uint8_t *buf, int buf_size)
{
   struct buffer_data *bd = (struct buffer_data *)opaque;

    buf_size = FFMIN(buf_size, bd->size);

    
    if (!buf_size)
        return AVERROR_EOF;
    memcpy(buf, bd->ptr, buf_size);
    bd->ptr  += buf_size;
    bd->size -= buf_size;
    return buf_size;
}

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
   Transcoder * test = Transcoder::Instance();
   std::vector<std::pair<int,int>> t{std::make_pair(256,257)};
   test->SetConfig("/home/johnny/tiral_go.ts","File",t);
   test->Process();
}
static void test3(){
   Transcoder * test = Transcoder::Instance();
   std::vector<std::pair<int,int>> t{std::make_pair(3042,3041)};
   test->SetConfig("/home/johnny/test.ts","File",t);
   test->Process();
   test->WriteEnd();
}

int main(){

  //  RunServer();
    test3();   
    return 0;

}