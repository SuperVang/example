#pragma once

#include "Registry.h"

#include <boost/make_shared.hpp>
#include <boost/process.hpp>
#include <grpc++/grpc++.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>

#include <automotive_ai/heart/heart_service.grpc.pb.h>

#include <fmt/format.h>

namespace aai_heart { namespace detail {

std::string printException(const std::exception_ptr& exception);
grpc::Status makeGrpcStatusFromException(const std::exception_ptr& exception, const char* func);

class StatusError : public std::runtime_error
{
public:
	explicit StatusError(grpc::StatusCode code = grpc::StatusCode::UNKNOWN, const std::string& message = std::string());
	explicit StatusError(const grpc::Status& status);

	template<typename ... Args>
	explicit StatusError(std::string format, Args&& ... args)
		: StatusError(grpc::StatusCode::INTERNAL, fmt::format(format, std::forward<Args>(args)...))
	{
	}

	grpc::StatusCode getCode() const;

private:
	grpc::StatusCode m_code = grpc::StatusCode::UNKNOWN;
};

struct ICompletion
{
	virtual ~ICompletion() = default;
	virtual void onComplete() = 0;
};

template<typename Response>
struct Completion : public ICompletion
{
	void onComplete() override
	{
		if (status.ok())
			promise.set_value(std::move(response));
		else
			promise.set_exception(std::make_exception_ptr(StatusError(status)));
	}

	grpc::ClientContext context;
	grpc::Status status;
	Response response;
	std::promise<Response> promise;
};

template<typename Request, typename Response, typename Stub>
using StubAsyncMethod = std::unique_ptr<grpc::ClientAsyncResponseReader<Response>>(Stub::*)(grpc::ClientContext*, const Request&, grpc::CompletionQueue*);

template<typename Request, typename Response, typename Stub>
std::future<Response> postStubAsyncMethod(
	grpc::CompletionQueue& cq,
	Stub& stub,
	StubAsyncMethod<Request, Response, Stub> method,
	const Request& request,
	boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
)
{
	auto tag = new Completion<Response>();
	auto res = tag->promise.get_future();
	if (deadline)
		tag->context.set_deadline(*deadline);
	auto reader = (stub.*method)(&tag->context, request, &cq);
	reader->Finish(&tag->response, &tag->status, (ICompletion*)tag);
	return res;
}

/**
 * @brief inherit the IHeart and gprc heart service so to hide the detail of service.
 * maintain registry to manager the other services loaded in heart service.
 * maintain event as callback functions which be implemented by user
 * 
 */
class Heart final : public IHeart, private ::automotive_ai::heart::Heart::Service
{
public:

	explicit Heart(IEvents& events);
	~Heart();

	virtual std::future<void> run(const std::string& address) final;
	virtual IRegistry& getRegistry() final;
	virtual void shutdown() final;

private:

	void loop(const std::string& address, std::promise<void> promise);

	grpc::Status Report(
		grpc::ServerContext* context,
		const ::automotive_ai::heart::ReportReq* request,
		::automotive_ai::heart::ReportRes* response
	) override;

	grpc::Status Init(
		grpc::ServerContext* context,
		const ::automotive_ai::heart::InitReq* request,
		::automotive_ai::heart::InitRes* response
	) override;

	grpc::Status Serve(
		grpc::ServerContext* context,
		const ::automotive_ai::heart::ServeReq* request,
		::automotive_ai::heart::ServeRes* response
	) override;

	grpc::Status Connect(
		grpc::ServerContext* context,
		const ::automotive_ai::heart::ConnectReq* request,
		::automotive_ai::heart::ConnectRes* response
	) override;

	grpc::Status Start(
		grpc::ServerContext* context,
		const ::automotive_ai::heart::StartReq* request,
		::automotive_ai::heart::StartRes* response
	) override;

	grpc::Status Step(
		grpc::ServerContext* context,
		const ::automotive_ai::heart::StepReq* request,
		::automotive_ai::heart::StepRes* response
	) override;

	grpc::Status Stop(
		grpc::ServerContext* context,
		const ::automotive_ai::heart::StopReq* request,
		::automotive_ai::heart::StopRes* response
	) override;

	std::vector<std::unique_ptr<grpc::Server>> mServers;
	bool mIsStopped = true;
	IEvents& mEvents;
	Registry mRegistry;
	grpc::CompletionQueue mCq;
};

} }
