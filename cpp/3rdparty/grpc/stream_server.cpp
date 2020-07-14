
#include <iostream>

#include "video.grpc.pb.h"
#include <grpc++/grpc++.h>

class VideoImpl : public books::VideoService::Service
{
public:
	
	
	~VideoImpl () override
	{
	
	}
	
	grpc::Status StreamOut (::grpc::ServerContext *context, const ::books::VideoRequest *request,
							::grpc::ServerWriter<::books::VideoResponse> *writer) override
	{
		writer->SendInitialMetadata();
		for (int i = 0; i < request->times(); i++)
		{
			::books::VideoResponse msg;
			msg.set_num(i);
			std::cout << "StreamOut Send" << i << std::endl;
			writer->Write(msg);
		}
		return grpc::Status::OK;
	}
	
	grpc::Status StreamIn (::grpc::ServerContext *context,
						   ::grpc::ServerReader<::books::VideoRequest> *reader,
						   ::books::VideoResponse *response) override
	{
		::books::VideoRequest req;
		int times = 0;
		while (reader->Read(&req))
		{
			std::cout << "StreamIn Receive " << req.times() << std::endl;
			times += req.times();
		}
		std::cout<<"StreamIn calculate "<<times<<std::endl;
		response->set_num(times);
		std::cout << "StreamIn Send "<< response->num() << std::endl;
		return grpc::Status::OK;
	}
	
	grpc::Status StreamInOut (::grpc::ServerContext *context,
							  ::grpc::ServerReaderWriter<::books::VideoResponse, ::books::VideoRequest> *stream) override
	{
		::books::VideoRequest req;
		while (stream->Read(&req))
		{
			::books::VideoResponse resp;
			std::cout << "StreamInOut Receive " << req.times() << std::endl;
			resp.set_num(req.times() * 2);
			std::cout << "StreamInOut Send " << resp.num() << std::endl;
			stream->Write(resp);
		}
		return grpc::Status::OK;
	}
};


int main ()
{
	std::string address = "127.0.0.1:50051";
	VideoImpl impl;
	grpc::ServerBuilder builder;
	builder.AddListeningPort(address, grpc::InsecureServerCredentials());
	builder.RegisterService(&impl);
	
	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	std::cout << "Server Listening On " << address << std::endl;
	server->Wait();
}
