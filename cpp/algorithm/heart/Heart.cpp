#include <aai_heart_src/Heart.h>

namespace aai_heart { namespace detail {

std::string printException(const std::exception_ptr& exception)
{
	try
	{
		if(auto ex = std::current_exception())
			std::rethrow_exception(std::move(ex));
	}
	catch(const std::exception& e)
	{
		return e.what();
	}
	catch(...)
	{
		return "Unknown exception";
	}
	return {};
}

grpc::Status makeGrpcStatusFromException(const std::exception_ptr& exception, const char* func)
{
	try
	{
		if(auto ex = std::current_exception())
			std::rethrow_exception(std::move(ex));
	}
	catch(...)
	{
		return grpc::Status(
			grpc::StatusCode::INTERNAL,
			fmt::format("{}: {}", func, printException(exception))
		);
	}
	return grpc::Status::OK;
}


StatusError::StatusError(grpc::StatusCode code, const std::string& message)
	: std::runtime_error(message), m_code(code)
{
}


StatusError::StatusError(const grpc::Status& status)
	: StatusError(status.error_code(), status.error_message())
{

}

grpc::StatusCode StatusError::getCode() const
{
	return m_code;
}

#if 0
inline std::string print_message(const ::google::protobuf::Message& message)
{
	::google::protobuf::util::JsonPrintOptions json_options;
	json_options.add_whitespace = true;
	json_options.always_print_primitive_fields = true;
	json_options.always_print_enums_as_ints = true;
	json_options.preserve_proto_field_names = false;
	std::string result;
	::google::protobuf::util::MessageToJsonString(message, &result, json_options);
	return result;
}

inline void print_call(
	const ::google::protobuf::Message& request,
	const ::google::protobuf::Message& response,
	const char* func)
{
	std::cout << ">>>>>> ---------------------------- call ----------------------------" << std::endl;
	std::cout << func << std::endl;
	std::cout << print_message(request) << std::endl;
	std::cout << print_message(response) << std::endl;
	std::cout << "<<<<<< ---------------------------- call ----------------------------" << std::endl;
}

inline void print_error(
	const ::google::protobuf::Message& request,
	const std::string& message,
	const char* func)
{
	std::cout << ">>>>>> ---------------------------- error ----------------------------" << std::endl;
	std::cout << func << std::endl;
	std::cout << print_message(request) << std::endl;
	std::cout << message << std::endl;
	std::cout << "<<<<<< ---------------------------- exception ----------------------------" << std::endl;
}

inline void print_exception(
	const ::google::protobuf::Message& request,
	const std::exception_ptr& exception,
	const char* func)
{
	std::cout << ">>>>>> ---------------------------- exception ----------------------------" << std::endl;
	std::cout << func << std::endl;
	std::cout << print_message(request) << std::endl;
	std::cout << printException(std::current_exception()) << std::endl;
	std::cout << "<<<<<< ---------------------------- exception ----------------------------" << std::endl;
}
#endif

class NoReusePortOption : public grpc::ServerBuilderOption
{
public:
	void UpdateArguments(grpc::ChannelArguments* args) override
	{
		args->SetInt(GRPC_ARG_ALLOW_REUSEPORT, 0);
	}

	void UpdatePlugins(std::vector<std::unique_ptr<grpc::ServerBuilderPlugin>>* plugins) override
	{
		(void)plugins;
	}
};

inline void complete(grpc::CompletionQueue& cq)
{
	void* tag = nullptr;
	bool ok = false;

	while(cq.Next(&tag, &ok))
	{
		if(!ok)
			throw Error("");

		try
		{
			std::unique_ptr<ICompletion> completion((ICompletion*)tag);
			completion->onComplete();
		}
		catch(...)
		{
		}
	}
}


Heart::Heart(IEvents& events)
	: mEvents(events)
{

}

Heart::~Heart()
{
	shutdown();
}

std::future<void> Heart::run(const std::string& address)
{
	auto promise = std::promise<void>();
	auto future = promise.get_future();

	auto result = std::async(
		std::launch::async,
		[this, address, promise = std::move(promise)]() mutable {
			loop(address, std::move(promise));
		}
	);

	future.get();
	return result;
}

aai_heart::IRegistry & Heart::getRegistry()
{
	return mRegistry;
}

void Heart::shutdown()
{
	mCq.Shutdown();
}

void Heart::loop(const std::string& address, std::promise<void> promise)
{
	try
	{
		// fmt::print("Heart.init {} (pid: {:d})\n", address, boost::this_process::get_id());

		grpc::ServerBuilder builder;
		builder.AddListeningPort(address, grpc::InsecureServerCredentials());
		builder.SetOption(std::make_unique<NoReusePortOption>());
		builder.RegisterService(this);

		std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
		promise.set_value();
		complete(mCq);

		server->Shutdown();
		server->Wait();
		server.reset();
	}
	catch(...)
	{
		promise.set_exception(std::current_exception());
	}
}

grpc::Status Heart::Report(
	grpc::ServerContext* context,
	const ::automotive_ai::heart::ReportReq* request,
	::automotive_ai::heart::ReportRes* response
)
{
	try
	{
		const std::string& content = request->content();
		if(!content.empty())
			response->set_content(mEvents.onReport(content));
	}
	catch(...)
	{
		return makeGrpcStatusFromException(std::current_exception(), BOOST_CURRENT_FUNCTION);
	}
	return grpc::Status::OK;
}

grpc::Status Heart::Init(
	grpc::ServerContext* context, 
	const ::automotive_ai::heart::InitReq* request, 
	::automotive_ai::heart::InitRes* response
)
{
	try
	{
		::automotive_ai::heart::StopReq stopReq;
		::automotive_ai::heart::StopRes stopRes;
		Stop(context, &stopReq, &stopRes);
		mIsStopped = false;

		mEvents.onInit(request->config());

		const auto& stubs = mRegistry.getStubs();
		response->mutable_stubs()->insert(stubs.begin(), stubs.end());

		const auto& services = mRegistry.getServices();
		response->mutable_services()->insert(services.begin(), services.end());
	}
	catch(...)
	{
		return makeGrpcStatusFromException(std::current_exception(), BOOST_CURRENT_FUNCTION);
	}
	return grpc::Status::OK;
}

grpc::Status Heart::Serve(
	grpc::ServerContext* context, 
	const ::automotive_ai::heart::ServeReq* request, 
	::automotive_ai::heart::ServeRes* response
)
{
	try
	{
		assert(mServers.empty());
		for(auto& i : mRegistry.applyServiceMapping(request->mapping()))
		{
			const Registry::Addr& addr = i.first;
			grpc::ServerBuilder& builder = i.second;
			builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
			builder.SetOption(std::make_unique<NoReusePortOption>());
			mServers.emplace_back(builder.BuildAndStart());
		}
	}
	catch(...)
	{
		return makeGrpcStatusFromException(std::current_exception(), BOOST_CURRENT_FUNCTION);
	}
	return grpc::Status::OK;
}

grpc::Status Heart::Connect(
	grpc::ServerContext* context, 
	const ::automotive_ai::heart::ConnectReq* request, 
	::automotive_ai::heart::ConnectRes* response
)
{
	try
	{
		const auto deadline = std::chrono::system_clock::now() + std::chrono::seconds(5);
		const auto& mapping = request->mapping();

		std::map<Registry::Addr, std::shared_ptr<grpc::Channel>> channels;
		for(auto& i : mRegistry.getStubs())
		{
			const Registry::Addr& addr = mapping.at(i.first);

			std::shared_ptr<grpc::Channel>& channel = channels[addr];
			if(channel)
				continue;

			channel = ::grpc::CreateChannel(addr, ::grpc::InsecureChannelCredentials());
			if(!channel->WaitForConnected(deadline))
				throw Error("Failed to connect '{}'", addr);
		}

		mRegistry.applyStubMapping(mCq, mapping, channels);
	}
	catch(...)
	{
		return makeGrpcStatusFromException(std::current_exception(), BOOST_CURRENT_FUNCTION);
	}
	return grpc::Status::OK;
}

grpc::Status Heart::Start(
	grpc::ServerContext* context, 
	const ::automotive_ai::heart::StartReq* request, 
	::automotive_ai::heart::StartRes* response
)
{
	try
	{
		mEvents.onStart();
	}
	catch(...)
	{
		return makeGrpcStatusFromException(std::current_exception(), BOOST_CURRENT_FUNCTION);
	}
	return grpc::Status::OK;
}

grpc::Status Heart::Step(
	grpc::ServerContext* context, 
	const ::automotive_ai::heart::StepReq* request, 
	::automotive_ai::heart::StepRes* response
)
{
	try
	{
		response->set_enough_please_stop(!mEvents.onStep(std::chrono::milliseconds(request->stamp())));
	}
	catch(...)
	{
		return makeGrpcStatusFromException(std::current_exception(), BOOST_CURRENT_FUNCTION);
	}
	return grpc::Status::OK;
}

grpc::Status Heart::Stop(
	grpc::ServerContext* context, 
	const ::automotive_ai::heart::StopReq* request, 
	::automotive_ai::heart::StopRes* response
)
{
	try
	{
		std::exception_ptr ex;
		if(!mIsStopped)
		{
			try
			{
				mEvents.onStop();
			}
			catch(...)
			{
				ex = std::current_exception();
			}
			mIsStopped = true;
		}

		mRegistry.reset();

		const auto deadline = std::chrono::system_clock::now() + std::chrono::seconds(5);
		for(std::unique_ptr<grpc::Server>& server : mServers)
		{
			server->Shutdown(deadline);
			server->Wait();
		}
		mServers.clear();

		if(ex)
			std::rethrow_exception(ex);
	}
	catch(...)
	{
		return makeGrpcStatusFromException(std::current_exception(), BOOST_CURRENT_FUNCTION);
	}
	return grpc::Status::OK;
}

} }

namespace aai_heart {

std::shared_ptr<IHeart> makeHeart(IEvents& events)
{
	return std::make_shared<detail::Heart>(events);
}

}
