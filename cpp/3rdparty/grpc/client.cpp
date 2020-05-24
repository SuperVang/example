

#include <iostream>

#include <grpc++/grpc++.h>

#include "foo.pb.h"
#include "foo.grpc.pb.h"

int main()
{
    std::string address="0.0.0.0:50051";
    std::shared_ptr<grpc::Channel> chan=grpc::CreateChannel(address,grpc::InsecureChannelCredentials());

    std::unique_ptr<EchoService::Stub> stub=EchoService::NewStub(chan);

    FooRequest req;
    req.set_text("bin");
    req.set_times(2);

    FooResponse res;

    grpc::ClientContext context;

    grpc::Status status=stub->Foo(&context,req,&res);

    if(status.ok())
        std::cout<<res.text()<<std::endl;

    return 0;
}
