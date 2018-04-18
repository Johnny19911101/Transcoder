#include "RPCServer.h"

using namespace Noovo;

struct buffer_data {
    uint8_t *ptr;
    size_t size; ///< size left in the buffer
};
static int read_packet(void *opaque, uint8_t *buf, int buf_size)
{
   struct buffer_data *bd = (struct buffer_data *)opaque;

    buf_size = FFMIN(buf_size, bd->size);

    
    if (!buf_size)
        return AVERROR_EOF;
    memcpy(buf, bd->ptr, buf_size);
    bd->ptr  += buf_size;
    bd->size -= buf_size;
    return buf_size;
}


ServiceImpl::ServiceImpl() : _trans(Transcoder::Instance()){
   
}
grpc::Status ServiceImpl::Setchannel(grpc::ServerContext* context, const transcode_rpc::PIDS* request,transcode_rpc::channel_list* reply)  {
    std::pair<int,int> channel=std::make_pair(request->video(),request->audio());
    _channel_list.push_back(channel);
    std::string reply_list;
    for(auto it=_channel_list.begin();it!=_channel_list.end();it++){
        reply_list = " channel : video" +std::to_string(it->first)+std::to_string(it->second);
    }
    reply->set_list(reply_list);
    return grpc::Status::OK;
}
grpc::Status ServiceImpl::Avio(grpc::ServerContext* context, const transcode_rpc::avio_type* request,transcode_rpc::StateReply* reply)  {
    int ret;
    ///////////////////////////////
    buffer_data  bd;
    size_t buffer_size;
    uint8_t* buffer;
    ret = av_file_map("/home/johnny/tiral_go.ts", &buffer, &buffer_size, 0, NULL);
    bd.ptr=buffer;
    bd.size =buffer_size;

    ///////////////////////////////
    if(request->inputtype() == 0)
        ret = _trans->InitalAvio(2048,&bd,&read_packet,"live.m3u8",_channel_list);
    else if(request->inputtype() == 1)
        _trans->SetConfig(request->file(),"File",_channel_list);
    reply->set_reply(_trans->ReturnStateName());
    return grpc::Status::OK;
}
grpc::Status ServiceImpl::Process(grpc::ServerContext* context, const transcode_rpc::empty* request,transcode_rpc::StateReply* reply)  {
    _tranprocess =_trans->Thread_process();
    reply->set_reply(_trans->ReturnStateName());
    return grpc::Status::OK;
}
grpc::Status ServiceImpl::State(grpc::ServerContext* context, const transcode_rpc::empty* request,transcode_rpc::StateReply* reply)  {
    reply->set_reply(_trans->ReturnStateName());
    return grpc::Status::OK;
}
grpc::Status ServiceImpl::Stop(grpc::ServerContext* context, const transcode_rpc::empty* request,transcode_rpc::StateReply* reply)  {
    int i=_trans->StopProcess();
    reply->set_reply(_trans->ReturnStateName());
    return grpc::Status::OK;
}