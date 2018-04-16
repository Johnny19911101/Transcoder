#include <thread>
#include <string>

#include "transcode_rpc.grpc.pb.h"
#include "Transcoder.h"

#include <grpc++/grpc++.h>

namespace Noovo{
    class ServiceImpl final : public transcode_rpc::Transcode::Service {
        public:
            ServiceImpl();
            grpc::Status setchannel(grpc::ServerContext* context, const transcode_rpc::PIDS* request,transcode_rpc::channel_list* reply) override;
            grpc::Status avio(grpc::ServerContext* context, const transcode_rpc::avio_type* request,transcode_rpc::StateReply* reply) override;
            grpc::Status process(grpc::ServerContext* context, const transcode_rpc::empty* request,transcode_rpc::StateReply* reply) override;
            grpc::Status state(grpc::ServerContext* context, const transcode_rpc::empty* request,transcode_rpc::StateReply* reply) override;
            grpc::Status stop(grpc::ServerContext* context, const transcode_rpc::empty* request,transcode_rpc::StateReply* reply) override;
        private:    
            std::vector<std::pair<int,int>> _channel_list;
            Transcoder* _trans;
            std::thread _tranprocess;
    };
}