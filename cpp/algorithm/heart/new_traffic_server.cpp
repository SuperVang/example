#include <grpc++/grpc++.h>

#include <automotive_ai/world/new_traffic_service.grpc.pb.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp> // I can has C++17?
#include <boost/optional.hpp>
#include <boost/program_options.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <iostream>
#include <memory>
#include <string>

#include "IHeart.h"

namespace gpb = google::protobuf;

namespace aac = automotive_ai::common;
namespace aaf = aai_framework;

using namespace aai_simcommon;

using namespace automotive_ai::common;

class NewTrafficServiceImpl final
    : public automotive_ai::world::NewTrafficService::Service
{
public:
    static ::google::protobuf::Empty gl_empty;
  NewTrafficServiceImpl(
  {
        std::cout << "Start Init New Traffic Service..." << std::endl;
        std::cout << "New Traffic Service Init Done!" << std::endl;
  }

 private:

  grpc::Status ReportTraffic(
      grpc::ServerContext*,
      const automotive_ai::common::NewTrafficVehicles* request,
      ::google::protobuf::Empty*) override
  {
        return grpc::Status::OK;
  }

  grpc::Status GetTraffic(
      grpc::ServerContext*, const google::protobuf::Empty*,
      automotive_ai::common::NewTrafficVehicles* reply) override
  {
        return grpc::Status::OK;
  }

  grpc::Status StepNextEnd(grpc::ServerContext*, const google::protobuf::Empty*,
                           google::protobuf::Empty*) override
  {
        return grpc::Status::OK;
  }
};

::google::protobuf::Empty NewTrafficServiceImpl::gl_empty{};

int RunWithHeart(const std::string &heart_url, ::automotive_ai::world::NewTrafficService::Service &service)
{
    struct Events final : public aai_heart::IEvents
    {
        explicit Events(::automotive_ai::world::NewTrafficService::Service &service)
            : service_(service), mImpl(aai_heart::makeHeart(*this))
        {
        }
        std::future<void> run(const std::string &address)
        {
            return mImpl->run(address);
        }
        void onInit(const std::string &config_text) final
        {
            std::cout << "Start Heart Init..." << std::endl;
            aai_cg::Ht::Config config = aai_cg::Ht::parseConfig(config_text);
            using Service = ::automotive_ai::world::NewTrafficService::Service;
            std::shared_ptr<Service> wrapped{&service_, [](Service *) {}};

            aai_heart::IRegistry &registry = mImpl->getRegistry();
            registry.registerServiceNewTraffic(
                config.component_names.new_traffic,
                aai_heart::makeServiceNewTrafficFromGrpc(wrapped));
            std::cout << "Heart Init Done." << std::endl;
        }

        std::shared_ptr<aai_heart::IHeart> mImpl;
        ::automotive_ai::world::NewTrafficService::Service &service_;
    };

    try
    {
        Events heart{service};
        heart.run(heart_url);
        return 0;
    }
    catch (const std::exception &ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unknown exception" << std::endl;
    }

    return 1;
}

int main(int ac, char **av)
{

    std::string server_addr = "127.0.0.1:50058";
    std::string heart_url = "127.0.0.1:50056";

    NewTrafficServiceImpl service;

    if (heart_url)
        return RunWithHeart(heart_url, service);

    std::cout << "Start Server Build" << std::endl;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_addr << std::endl;

    server->Wait();

    return 0;
}
