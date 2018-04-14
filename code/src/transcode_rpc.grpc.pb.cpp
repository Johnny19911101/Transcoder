// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: transcode_rpc.proto

#include "transcode_rpc.pb.h"
#include "transcode_rpc.grpc.pb.h"

#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace transcode_rpc {

static const char* Transcode_method_names[] = {
  "/transcode_rpc.Transcode/setchannel",
  "/transcode_rpc.Transcode/avio",
  "/transcode_rpc.Transcode/process",
};

std::unique_ptr< Transcode::Stub> Transcode::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Transcode::Stub> stub(new Transcode::Stub(channel));
  return stub;
}

Transcode::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_setchannel_(Transcode_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_avio_(Transcode_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_process_(Transcode_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Transcode::Stub::setchannel(::grpc::ClientContext* context, const ::transcode_rpc::PIDS& request, ::transcode_rpc::channel_list* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_setchannel_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::transcode_rpc::channel_list>* Transcode::Stub::AsyncsetchannelRaw(::grpc::ClientContext* context, const ::transcode_rpc::PIDS& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::transcode_rpc::channel_list>::Create(channel_.get(), cq, rpcmethod_setchannel_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::transcode_rpc::channel_list>* Transcode::Stub::PrepareAsyncsetchannelRaw(::grpc::ClientContext* context, const ::transcode_rpc::PIDS& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::transcode_rpc::channel_list>::Create(channel_.get(), cq, rpcmethod_setchannel_, context, request, false);
}

::grpc::Status Transcode::Stub::avio(::grpc::ClientContext* context, const ::transcode_rpc::avio_type& request, ::transcode_rpc::StateReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_avio_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>* Transcode::Stub::AsyncavioRaw(::grpc::ClientContext* context, const ::transcode_rpc::avio_type& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::transcode_rpc::StateReply>::Create(channel_.get(), cq, rpcmethod_avio_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>* Transcode::Stub::PrepareAsyncavioRaw(::grpc::ClientContext* context, const ::transcode_rpc::avio_type& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::transcode_rpc::StateReply>::Create(channel_.get(), cq, rpcmethod_avio_, context, request, false);
}

::grpc::Status Transcode::Stub::process(::grpc::ClientContext* context, const ::transcode_rpc::empty& request, ::transcode_rpc::StateReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_process_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>* Transcode::Stub::AsyncprocessRaw(::grpc::ClientContext* context, const ::transcode_rpc::empty& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::transcode_rpc::StateReply>::Create(channel_.get(), cq, rpcmethod_process_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>* Transcode::Stub::PrepareAsyncprocessRaw(::grpc::ClientContext* context, const ::transcode_rpc::empty& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::transcode_rpc::StateReply>::Create(channel_.get(), cq, rpcmethod_process_, context, request, false);
}

Transcode::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Transcode_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Transcode::Service, ::transcode_rpc::PIDS, ::transcode_rpc::channel_list>(
          std::mem_fn(&Transcode::Service::setchannel), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Transcode_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Transcode::Service, ::transcode_rpc::avio_type, ::transcode_rpc::StateReply>(
          std::mem_fn(&Transcode::Service::avio), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Transcode_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Transcode::Service, ::transcode_rpc::empty, ::transcode_rpc::StateReply>(
          std::mem_fn(&Transcode::Service::process), this)));
}

Transcode::Service::~Service() {
}

::grpc::Status Transcode::Service::setchannel(::grpc::ServerContext* context, const ::transcode_rpc::PIDS* request, ::transcode_rpc::channel_list* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Transcode::Service::avio(::grpc::ServerContext* context, const ::transcode_rpc::avio_type* request, ::transcode_rpc::StateReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Transcode::Service::process(::grpc::ServerContext* context, const ::transcode_rpc::empty* request, ::transcode_rpc::StateReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace transcode_rpc
