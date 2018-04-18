#include <thread>
#include <string>

#include "transcode_rpc.grpc.pb.h"
#include "Transcoder.h"

#include <grpc++/grpc++.h>

namespace Noovo{
    class ServiceImpl final : public transcode_rpc::Transcode::Service {
        public:
            ServiceImpl();
            grpc::Status Setchannel(grpc::ServerContext* context, const transcode_rpc::PIDS* request,transcode_rpc::channel_list* reply) override;
            grpc::Status Avio(grpc::ServerContext* context, const transcode_rpc::avio_type* request,transcode_rpc::StateReply* reply) override;
            grpc::Status Process(grpc::ServerContext* context, const transcode_rpc::empty* request,transcode_rpc::StateReply* reply) override;
            grpc::Status State(grpc::ServerContext* context, const transcode_rpc::empty* request,transcode_rpc::StateReply* reply) override;
            grpc::Status Stop(grpc::ServerContext* context, const transcode_rpc::empty* request,transcode_rpc::StateReply* reply) override;
        private:    
            std::vector<std::pair<int,int>> _channel_list;
            Transcoder* _trans;
            std::thread _tranprocess;
    };
}