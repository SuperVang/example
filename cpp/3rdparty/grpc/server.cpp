#include <iostream>

#include "foo.pb.h"
#include "foo.grpc.pb.h"
#include <grpc++/grpc++.h>

class EchoServiceImpl final : public ::EchoService::Service {
   public:
    grpc::Status Foo(grpc::ServerContext *context,const FooRequest *request,
    FooResponse *reply)override{
        std::string str = request->text();

        std::string tmp = str;
        for (int i = 1; i < request->times(); i++) str += (" " + tmp);
        reply->set_text(str);
        reply->set_result(true);
        return grpc::Status::OK;
    }

};

int main(int argc, char *argv[]) {
    std::string address="127.0.0.1:50051";
    EchoServiceImpl impl;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address,grpc::InsecureServerCredentials());
    builder.RegisterService(&impl);
    
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout<<"Server Listening On "<<address<<std::endl;
    server->Wait();

    return 0;
}
