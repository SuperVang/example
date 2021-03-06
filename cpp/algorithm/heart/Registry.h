// Generated by the 'ai.aai.core/scripts/autogen/generate_cm.py' script.
// If you make any local change, they will be lost.

#pragma once

#include "IHeart.h"
#include <boost/smart_ptr/atomic_shared_ptr.hpp>
#include <grpc++/grpc++.h>


namespace aai_heart { namespace detail {

/**
 * @brief inherint IServiceNewTraffic but just implement the exception of promise
 * do not implement the real service process
 * 
 */
class DummyServiceNewTraffic : public IServiceNewTraffic
{
public:
    virtual ::automotive_ai::common::NewTrafficVehicles
        OnGetTraffic(
        const ::google::protobuf::Empty& request);

    virtual void OnGetTraffic(
        const ::google::protobuf::Empty& request,
        std::promise<::automotive_ai::common::NewTrafficVehicles> promise
    ) override;

    virtual ::google::protobuf::Empty
        OnReportTraffic(
        const ::automotive_ai::common::NewTrafficVehicles& request);

    virtual void OnReportTraffic(
        const ::automotive_ai::common::NewTrafficVehicles& request,
        std::promise<::google::protobuf::Empty> promise
    ) override;

    virtual ::google::protobuf::Empty
        OnStepNextEnd(
        const ::google::protobuf::Empty& request);

    virtual void OnStepNextEnd(
        const ::google::protobuf::Empty& request,
        std::promise<::google::protobuf::Empty> promise
    ) override;
};

/**
 * @brief inherit grpc service but just extend the origin interface with another formula
 * do not implement the real service process
 * 
 */
class GrpcDummyServiceNewTraffic : public ::automotive_ai::world::NewTrafficService::Service
{
public:
    virtual ::automotive_ai::common::NewTrafficVehicles
        GetTraffic(
        const ::google::protobuf::Empty& request);

    virtual grpc::Status GetTraffic(
        grpc::ServerContext* context,
        const ::google::protobuf::Empty* request,
        ::automotive_ai::common::NewTrafficVehicles* response
    ) override;

    virtual ::google::protobuf::Empty
        ReportTraffic(
        const ::automotive_ai::common::NewTrafficVehicles& request);

    virtual grpc::Status ReportTraffic(
        grpc::ServerContext* context,
        const ::automotive_ai::common::NewTrafficVehicles* request,
        ::google::protobuf::Empty* response
    ) override;

    virtual ::google::protobuf::Empty
        StepNextEnd(
        const ::google::protobuf::Empty& request);

    virtual grpc::Status StepNextEnd(
        grpc::ServerContext* context,
        const ::google::protobuf::Empty* request,
        ::google::protobuf::Empty* response
    ) override;
};

/**
 * @brief inherit grpc service and extend origin interface with deadline
 * it maintain an IServiceNewTraffic, and call it's interface in respective interface
 * need to notice that even the origin interface will use deadline from server context
 * 
 */
class GrpcFromServiceNewTraffic : public ::automotive_ai::world::NewTrafficService::Service
{
public:
    explicit GrpcFromServiceNewTraffic(std::shared_ptr<IServiceNewTraffic> proxy = nullptr);

    void setProxy(std::shared_ptr<IServiceNewTraffic> proxy);

    virtual ::automotive_ai::common::NewTrafficVehicles
        GetTraffic(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none);

    virtual grpc::Status GetTraffic(
        grpc::ServerContext* context,
        const ::google::protobuf::Empty* request,
        ::automotive_ai::common::NewTrafficVehicles* response
    ) override;

    virtual ::google::protobuf::Empty
        ReportTraffic(
        const ::automotive_ai::common::NewTrafficVehicles& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none);

    virtual grpc::Status ReportTraffic(
        grpc::ServerContext* context,
        const ::automotive_ai::common::NewTrafficVehicles* request,
        ::google::protobuf::Empty* response
    ) override;

    virtual ::google::protobuf::Empty
        StepNextEnd(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none);

    virtual grpc::Status StepNextEnd(
        grpc::ServerContext* context,
        const ::google::protobuf::Empty* request,
        ::google::protobuf::Empty* response
    ) override;

private:
    std::shared_ptr<IServiceNewTraffic> mProxy;
};

/**
 * @brief inherit grpc service and extend origin interface with deadline
 * it maintain an IStubNewTraffic, and call it's interface in respective interface
 * need to notice that even the origin interface will use deadline from server context
 * 
 */
class GrpcProxyServiceNewTraffic : public ::automotive_ai::world::NewTrafficService::Service
{
public:
    explicit GrpcProxyServiceNewTraffic(std::shared_ptr<IStubNewTraffic> proxy = nullptr);

    void setProxy(std::shared_ptr<IStubNewTraffic> proxy);

    virtual ::automotive_ai::common::NewTrafficVehicles
        GetTraffic(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none);

    virtual grpc::Status GetTraffic(
        grpc::ServerContext* context,
        const ::google::protobuf::Empty* request,
        ::automotive_ai::common::NewTrafficVehicles* response
    ) override;

    virtual ::google::protobuf::Empty
        ReportTraffic(
        const ::automotive_ai::common::NewTrafficVehicles& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none);

    virtual grpc::Status ReportTraffic(
        grpc::ServerContext* context,
        const ::automotive_ai::common::NewTrafficVehicles* request,
        ::google::protobuf::Empty* response
    ) override;

    virtual ::google::protobuf::Empty
        StepNextEnd(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none);

    virtual grpc::Status StepNextEnd(
        grpc::ServerContext* context,
        const ::google::protobuf::Empty* request,
        ::google::protobuf::Empty* response
    ) override;

private:
    std::shared_ptr<IStubNewTraffic> mProxy;
};

/**
 * @brief inherit the IServiceNewTraffic and implement the interface via a grpc service 
 * call the interface inside proxy when run the respective service
 * wrap the origin synchronous interface to asynchronous one by put the result into promise
 */
class ServiceNewTrafficFromGrpc : public IServiceNewTraffic
{
public:
    explicit ServiceNewTrafficFromGrpc(std::shared_ptr<::automotive_ai::world::NewTrafficService::Service> proxy = nullptr);

    void setProxy(std::shared_ptr<::automotive_ai::world::NewTrafficService::Service> proxy);

    virtual void OnGetTraffic(
        const ::google::protobuf::Empty& request,
        std::promise<::automotive_ai::common::NewTrafficVehicles> promise
    ) override;

    virtual void OnReportTraffic(
        const ::automotive_ai::common::NewTrafficVehicles& request,
        std::promise<::google::protobuf::Empty> promise
    ) override;

    virtual void OnStepNextEnd(
        const ::google::protobuf::Empty& request,
        std::promise<::google::protobuf::Empty> promise
    ) override;

private:
    std::shared_ptr<::automotive_ai::world::NewTrafficService::Service> mProxy;
};

/**
 * @brief inherit IServiceNewTraffic and implement the interface via grpc stub interface
 * call the interface inside proxy when run the respective service
 * wrap the origin synchronous interface to asynchronous one by put the result into promise
 */
class ServiceNewTrafficFromGrpcStub : public IServiceNewTraffic
{
public:
    explicit ServiceNewTrafficFromGrpcStub(std::unique_ptr<::automotive_ai::world::NewTrafficService::StubInterface> proxy = nullptr);

    void setProxy(std::unique_ptr<::automotive_ai::world::NewTrafficService::StubInterface> proxy);

    virtual void OnGetTraffic(
        const ::google::protobuf::Empty& request,
        std::promise<::automotive_ai::common::NewTrafficVehicles> promise
    ) override;

    virtual void OnReportTraffic(
        const ::automotive_ai::common::NewTrafficVehicles& request,
        std::promise<::google::protobuf::Empty> promise
    ) override;

    virtual void OnStepNextEnd(
        const ::google::protobuf::Empty& request,
        std::promise<::google::protobuf::Empty> promise
    ) override;

private:
    boost::atomic_shared_ptr<::automotive_ai::world::NewTrafficService::StubInterface> mProxy;
};

/**
 * @brief inherit IStubNewTraffic with empty implementation
 * 
 */
class DummyStubNewTraffic : public IStubNewTraffic
{
public:
    virtual ::automotive_ai::common::NewTrafficVehicles
        GetTraffic(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual std::future<::automotive_ai::common::NewTrafficVehicles>
        GetTrafficAsync(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual ::google::protobuf::Empty
        ReportTraffic(
        const ::automotive_ai::common::NewTrafficVehicles& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual std::future<::google::protobuf::Empty>
        ReportTrafficAsync(
        const ::automotive_ai::common::NewTrafficVehicles& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual ::google::protobuf::Empty
        StepNextEnd(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual std::future<::google::protobuf::Empty>
        StepNextEndAsync(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;
};

/**
 * @brief inherit grpc stub interface with empty implementation
 * 
 */
class GrpcDummyStubNewTraffic : public ::automotive_ai::world::NewTrafficService::StubInterface
{
public:
    virtual grpc::Status GetTraffic(
        grpc::ClientContext* context,
        const ::google::protobuf::Empty& request,
        ::automotive_ai::common::NewTrafficVehicles* response
    ) override;

    virtual grpc::Status ReportTraffic(
        grpc::ClientContext* context,
        const ::automotive_ai::common::NewTrafficVehicles& request,
        ::google::protobuf::Empty* response
    ) override;

    virtual grpc::Status StepNextEnd(
        grpc::ClientContext* context,
        const ::google::protobuf::Empty& request,
        ::google::protobuf::Empty* response
    ) override;

private:
    virtual grpc::ClientAsyncResponseReaderInterface<::automotive_ai::common::NewTrafficVehicles>*
        AsyncGetTrafficRaw(
        grpc::ClientContext* context,
        const ::google::protobuf::Empty& request,
        grpc::CompletionQueue* cq
    );

    virtual grpc::ClientAsyncResponseReaderInterface<::automotive_ai::common::NewTrafficVehicles>*
        PrepareAsyncGetTrafficRaw(
        grpc::ClientContext* context,
        const ::google::protobuf::Empty& request,
        grpc::CompletionQueue* cq
    );

    virtual grpc::ClientAsyncResponseReaderInterface<::google::protobuf::Empty>*
        AsyncReportTrafficRaw(
        grpc::ClientContext* context,
        const ::automotive_ai::common::NewTrafficVehicles& request,
        grpc::CompletionQueue* cq
    );

    virtual grpc::ClientAsyncResponseReaderInterface<::google::protobuf::Empty>*
        PrepareAsyncReportTrafficRaw(
        grpc::ClientContext* context,
        const ::automotive_ai::common::NewTrafficVehicles& request,
        grpc::CompletionQueue* cq
    );

    virtual grpc::ClientAsyncResponseReaderInterface<::google::protobuf::Empty>*
        AsyncStepNextEndRaw(
        grpc::ClientContext* context,
        const ::google::protobuf::Empty& request,
        grpc::CompletionQueue* cq
    );

    virtual grpc::ClientAsyncResponseReaderInterface<::google::protobuf::Empty>*
        PrepareAsyncStepNextEndRaw(
        grpc::ClientContext* context,
        const ::google::protobuf::Empty& request,
        grpc::CompletionQueue* cq
    );
};

/**
 * @brief inherit IStubNewTraffic and use a grpc stub as proxy
 * the synchronous interface will be same as proxy
 * the asynchronous interface will call the asynchronous interface of stub
 * 
 */
class GrpcProxyStubNewTraffic : public IStubNewTraffic
{
public:
    explicit GrpcProxyStubNewTraffic(grpc::CompletionQueue& cq, const std::shared_ptr<grpc::Channel>& channel);

    virtual ::automotive_ai::common::NewTrafficVehicles
        GetTraffic(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual std::future<::automotive_ai::common::NewTrafficVehicles>
        GetTrafficAsync(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual ::google::protobuf::Empty
        ReportTraffic(
        const ::automotive_ai::common::NewTrafficVehicles& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual std::future<::google::protobuf::Empty>
        ReportTrafficAsync(
        const ::automotive_ai::common::NewTrafficVehicles& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual ::google::protobuf::Empty
        StepNextEnd(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual std::future<::google::protobuf::Empty>
        StepNextEndAsync(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

private:
    grpc::CompletionQueue& mCq;
    std::unique_ptr<::automotive_ai::world::NewTrafficService::Stub> mProxy;
};

/**
 * @brief inherit the grpc stub interface and use IStubNewTraffic as proxy
 * 
 */
class GrpcFromStubNewTraffic : public ::automotive_ai::world::NewTrafficService::StubInterface
{
public:
    explicit GrpcFromStubNewTraffic(std::shared_ptr<IStubNewTraffic> proxy = nullptr);

    void setProxy(std::shared_ptr<IStubNewTraffic> proxy);

    virtual grpc::Status GetTraffic(
        grpc::ClientContext* context,
        const ::google::protobuf::Empty& request,
        ::automotive_ai::common::NewTrafficVehicles* response
    ) override;

    virtual grpc::Status ReportTraffic(
        grpc::ClientContext* context,
        const ::automotive_ai::common::NewTrafficVehicles& request,
        ::google::protobuf::Empty* response
    ) override;

    virtual grpc::Status StepNextEnd(
        grpc::ClientContext* context,
        const ::google::protobuf::Empty& request,
        ::google::protobuf::Empty* response
    ) override;

private:
    std::shared_ptr<IStubNewTraffic> mProxy;

private:
    virtual grpc::ClientAsyncResponseReaderInterface<::automotive_ai::common::NewTrafficVehicles>*
        AsyncGetTrafficRaw(
        grpc::ClientContext* context,
        const ::google::protobuf::Empty& request,
        grpc::CompletionQueue* cq
    );

    virtual grpc::ClientAsyncResponseReaderInterface<::automotive_ai::common::NewTrafficVehicles>*
        PrepareAsyncGetTrafficRaw(
        grpc::ClientContext* context,
        const ::google::protobuf::Empty& request,
        grpc::CompletionQueue* cq
    );

    virtual grpc::ClientAsyncResponseReaderInterface<::google::protobuf::Empty>*
        AsyncReportTrafficRaw(
        grpc::ClientContext* context,
        const ::automotive_ai::common::NewTrafficVehicles& request,
        grpc::CompletionQueue* cq
    );

    virtual grpc::ClientAsyncResponseReaderInterface<::google::protobuf::Empty>*
        PrepareAsyncReportTrafficRaw(
        grpc::ClientContext* context,
        const ::automotive_ai::common::NewTrafficVehicles& request,
        grpc::CompletionQueue* cq
    );

    virtual grpc::ClientAsyncResponseReaderInterface<::google::protobuf::Empty>*
        AsyncStepNextEndRaw(
        grpc::ClientContext* context,
        const ::google::protobuf::Empty& request,
        grpc::CompletionQueue* cq
    );

    virtual grpc::ClientAsyncResponseReaderInterface<::google::protobuf::Empty>*
        PrepareAsyncStepNextEndRaw(
        grpc::ClientContext* context,
        const ::google::protobuf::Empty& request,
        grpc::CompletionQueue* cq
    );
};

/**
 * @brief inherit IStubNewTraffic and use IStubNewTraffic as proxy
 *  
 */
class ProxyStubNewTraffic : public IStubNewTraffic
{
public:
    explicit ProxyStubNewTraffic(std::shared_ptr<IStubNewTraffic> proxy = nullptr);

    void setProxy(std::shared_ptr<IStubNewTraffic> proxy);

    virtual ::automotive_ai::common::NewTrafficVehicles
        GetTraffic(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual std::future<::automotive_ai::common::NewTrafficVehicles>
        GetTrafficAsync(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual ::google::protobuf::Empty
        ReportTraffic(
        const ::automotive_ai::common::NewTrafficVehicles& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual std::future<::google::protobuf::Empty>
        ReportTrafficAsync(
        const ::automotive_ai::common::NewTrafficVehicles& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual ::google::protobuf::Empty
        StepNextEnd(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

    virtual std::future<::google::protobuf::Empty>
        StepNextEndAsync(
        const ::google::protobuf::Empty& request,
        boost::optional<std::chrono::system_clock::time_point> deadline = boost::none
    ) override;

private:
    std::shared_ptr<IStubNewTraffic> mProxy;
};

class Registry final : public IRegistry
{
public:
    using Type = std::string;
    using Name = std::string;
    using Addr = std::string;

    const std::map<Name, Type>& getStubs() const;
    const std::map<Name, Type>& getServices() const;

    #pragma region Register

    /**
     * @brief register a new ProxyStubNewTraffic with name into mStubsNewTraffic, or just return the ProxyStubNewTraffic with name
     * 
     * @param name 
     * @return std::shared_ptr<IStubNewTraffic> 
     */
    virtual std::shared_ptr<IStubNewTraffic> registerStubNewTraffic(const Name& name) final;

    /**
     * @brief register a new GrpcFromServiceNewTraffic with name which is converted from service
     * 
     * @param name 
     * @param service 
     */
    virtual void registerServiceNewTraffic(const Name& name, std::shared_ptr<IServiceNewTraffic> service) final;

    #pragma endregion

    /**
     * @brief use mServicesNewTraffic initilaize a <Addr, Grpc Server Builder> map
     * find same key from mServicesNewTraffic and mapping, register mServicesNewTraffic[key] to result[mapping[key]]
     * @param mapping 
     * @return std::map<Addr, grpc::ServerBuilder> 
     */
    std::map<Addr, grpc::ServerBuilder> applyServiceMapping(const ::google::protobuf::Map<std::string, std::string>& mapping);

    /**
     * @brief use cq and channels to initialize the proxy of mStubsNewTraffic whioh has same key with mapping
     * each proxy is a GrpcProxyStubNewTraffic which take cq and channels[mapping[key]]
     *  
     * @param cq 
     * @param mapping 
     * @param channels 
     */
    void applyStubMapping(
        grpc::CompletionQueue& cq,
        const ::google::protobuf::Map<std::string, std::string>& mapping,
        const std::map<Addr, std::shared_ptr<grpc::Channel>>& channels);

    void reset();

private:
    std::map<Name, Type> mStubs;
    std::map<Name, Type> mServices;

    #pragma region Variables

    std::map<Name, std::shared_ptr<ProxyStubNewTraffic> > mStubsNewTraffic;
    std::map<Name, std::shared_ptr<GrpcFromServiceNewTraffic> > mServicesNewTraffic;

    #pragma endregion
};

} }
