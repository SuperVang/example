
#include <iostream>
#include <thread>
#include "video.grpc.pb.h"
#include <grpc++/grpc++.h>

int main ()
{
	std::string address = "0.0.0.0:50051";
	std::shared_ptr<grpc::Channel> chan = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
	
	std::unique_ptr<books::VideoService::Stub> stub = books::VideoService::NewStub(chan);
	
	{
		grpc::ClientContext context;
		books::VideoRequest req;
		req.set_times(10);
		std::unique_ptr<::grpc::ClientReader<::books::VideoResponse>>
				cr = stub->StreamOut(&context, req);
		
		books::VideoResponse resp;
		while (cr->Read(&resp))
		{
			std::cout << "StreamOut Receive" << resp.num() << std::endl;
		}
		std::cout << "Stream Out Close" << std::endl;
	}
	
	{
		grpc::ClientContext context;
		books::VideoResponse resp;
		{
			std::unique_ptr<::grpc::ClientWriter<::books::VideoRequest>>
					cw = stub->StreamIn(&context, &resp);
			
			int sum = 0;
			for (int i = 0; i < 10; i++)
			{
				books::VideoRequest req;
				req.set_times(i);
				std::cout << "StreamIn Send " << req.times() << std::endl;
				cw->Write(req);
				sum += i;
			}
			cw->WritesDone();
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "StreamIn Receive " << resp.num() << std::endl;
		
	}
	
	{
		grpc::ClientContext context;
		std::unique_ptr<::grpc::ClientReaderWriter<::books::VideoRequest, ::books::VideoResponse>>
				rw = stub->StreamInOut(&context);
		
		for (int i = 0; i < 10; i++)
		{
			books::VideoRequest req;
			books::VideoResponse resp;
			
			req.set_times(i);
			rw->Write(req);
			rw->Read(&resp);
			std::cout << "StreamInOut Receive " << resp.num() << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		
	}
	
	
}