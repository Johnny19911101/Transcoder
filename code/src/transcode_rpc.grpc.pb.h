// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: transcode_rpc.proto
// Original file comments:
// Copyright 2015 gRPC authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef GRPC_transcode_5frpc_2eproto__INCLUDED
#define GRPC_transcode_5frpc_2eproto__INCLUDED

#include "transcode_rpc.pb.h"

#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace transcode_rpc {

// The greeting service definition.
class Transcode final {
 public:
  static constexpr char const* service_full_name() {
    return "transcode_rpc.Transcode";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status setchannel(::grpc::ClientContext* context, const ::transcode_rpc::PIDS& request, ::transcode_rpc::channel_list* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::channel_list>> Asyncsetchannel(::grpc::ClientContext* context, const ::transcode_rpc::PIDS& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::channel_list>>(AsyncsetchannelRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::channel_list>> PrepareAsyncsetchannel(::grpc::ClientContext* context, const ::transcode_rpc::PIDS& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::channel_list>>(PrepareAsyncsetchannelRaw(context, request, cq));
    }
    virtual ::grpc::Status avio(::grpc::ClientContext* context, const ::transcode_rpc::avio_type& request, ::transcode_rpc::StateReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::StateReply>> Asyncavio(::grpc::ClientContext* context, const ::transcode_rpc::avio_type& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::StateReply>>(AsyncavioRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::StateReply>> PrepareAsyncavio(::grpc::ClientContext* context, const ::transcode_rpc::avio_type& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::StateReply>>(PrepareAsyncavioRaw(context, request, cq));
    }
    virtual ::grpc::Status process(::grpc::ClientContext* context, const ::transcode_rpc::empty& request, ::transcode_rpc::StateReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::StateReply>> Asyncprocess(::grpc::ClientContext* context, const ::transcode_rpc::empty& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::StateReply>>(AsyncprocessRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::StateReply>> PrepareAsyncprocess(::grpc::ClientContext* context, const ::transcode_rpc::empty& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::StateReply>>(PrepareAsyncprocessRaw(context, request, cq));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::channel_list>* AsyncsetchannelRaw(::grpc::ClientContext* context, const ::transcode_rpc::PIDS& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::channel_list>* PrepareAsyncsetchannelRaw(::grpc::ClientContext* context, const ::transcode_rpc::PIDS& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::StateReply>* AsyncavioRaw(::grpc::ClientContext* context, const ::transcode_rpc::avio_type& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::StateReply>* PrepareAsyncavioRaw(::grpc::ClientContext* context, const ::transcode_rpc::avio_type& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::StateReply>* AsyncprocessRaw(::grpc::ClientContext* context, const ::transcode_rpc::empty& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::transcode_rpc::StateReply>* PrepareAsyncprocessRaw(::grpc::ClientContext* context, const ::transcode_rpc::empty& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status setchannel(::grpc::ClientContext* context, const ::transcode_rpc::PIDS& request, ::transcode_rpc::channel_list* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::transcode_rpc::channel_list>> Asyncsetchannel(::grpc::ClientContext* context, const ::transcode_rpc::PIDS& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::transcode_rpc::channel_list>>(AsyncsetchannelRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::transcode_rpc::channel_list>> PrepareAsyncsetchannel(::grpc::ClientContext* context, const ::transcode_rpc::PIDS& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::transcode_rpc::channel_list>>(PrepareAsyncsetchannelRaw(context, request, cq));
    }
    ::grpc::Status avio(::grpc::ClientContext* context, const ::transcode_rpc::avio_type& request, ::transcode_rpc::StateReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>> Asyncavio(::grpc::ClientContext* context, const ::transcode_rpc::avio_type& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>>(AsyncavioRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>> PrepareAsyncavio(::grpc::ClientContext* context, const ::transcode_rpc::avio_type& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>>(PrepareAsyncavioRaw(context, request, cq));
    }
    ::grpc::Status process(::grpc::ClientContext* context, const ::transcode_rpc::empty& request, ::transcode_rpc::StateReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>> Asyncprocess(::grpc::ClientContext* context, const ::transcode_rpc::empty& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>>(AsyncprocessRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>> PrepareAsyncprocess(::grpc::ClientContext* context, const ::transcode_rpc::empty& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>>(PrepareAsyncprocessRaw(context, request, cq));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientAsyncResponseReader< ::transcode_rpc::channel_list>* AsyncsetchannelRaw(::grpc::ClientContext* context, const ::transcode_rpc::PIDS& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::transcode_rpc::channel_list>* PrepareAsyncsetchannelRaw(::grpc::ClientContext* context, const ::transcode_rpc::PIDS& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>* AsyncavioRaw(::grpc::ClientContext* context, const ::transcode_rpc::avio_type& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>* PrepareAsyncavioRaw(::grpc::ClientContext* context, const ::transcode_rpc::avio_type& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>* AsyncprocessRaw(::grpc::ClientContext* context, const ::transcode_rpc::empty& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::transcode_rpc::StateReply>* PrepareAsyncprocessRaw(::grpc::ClientContext* context, const ::transcode_rpc::empty& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_setchannel_;
    const ::grpc::internal::RpcMethod rpcmethod_avio_;
    const ::grpc::internal::RpcMethod rpcmethod_process_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status setchannel(::grpc::ServerContext* context, const ::transcode_rpc::PIDS* request, ::transcode_rpc::channel_list* response);
    virtual ::grpc::Status avio(::grpc::ServerContext* context, const ::transcode_rpc::avio_type* request, ::transcode_rpc::StateReply* response);
    virtual ::grpc::Status process(::grpc::ServerContext* context, const ::transcode_rpc::empty* request, ::transcode_rpc::StateReply* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_setchannel : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_setchannel() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_setchannel() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status setchannel(::grpc::ServerContext* context, const ::transcode_rpc::PIDS* request, ::transcode_rpc::channel_list* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requestsetchannel(::grpc::ServerContext* context, ::transcode_rpc::PIDS* request, ::grpc::ServerAsyncResponseWriter< ::transcode_rpc::channel_list>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_avio : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_avio() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_avio() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status avio(::grpc::ServerContext* context, const ::transcode_rpc::avio_type* request, ::transcode_rpc::StateReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requestavio(::grpc::ServerContext* context, ::transcode_rpc::avio_type* request, ::grpc::ServerAsyncResponseWriter< ::transcode_rpc::StateReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_process : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_process() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_process() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status process(::grpc::ServerContext* context, const ::transcode_rpc::empty* request, ::transcode_rpc::StateReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requestprocess(::grpc::ServerContext* context, ::transcode_rpc::empty* request, ::grpc::ServerAsyncResponseWriter< ::transcode_rpc::StateReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_setchannel<WithAsyncMethod_avio<WithAsyncMethod_process<Service > > > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_setchannel : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_setchannel() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_setchannel() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status setchannel(::grpc::ServerContext* context, const ::transcode_rpc::PIDS* request, ::transcode_rpc::channel_list* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_avio : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_avio() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_avio() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status avio(::grpc::ServerContext* context, const ::transcode_rpc::avio_type* request, ::transcode_rpc::StateReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_process : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_process() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_process() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status process(::grpc::ServerContext* context, const ::transcode_rpc::empty* request, ::transcode_rpc::StateReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_setchannel : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_setchannel() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler< ::transcode_rpc::PIDS, ::transcode_rpc::channel_list>(std::bind(&WithStreamedUnaryMethod_setchannel<BaseClass>::Streamedsetchannel, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_setchannel() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status setchannel(::grpc::ServerContext* context, const ::transcode_rpc::PIDS* request, ::transcode_rpc::channel_list* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status Streamedsetchannel(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::transcode_rpc::PIDS,::transcode_rpc::channel_list>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_avio : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_avio() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler< ::transcode_rpc::avio_type, ::transcode_rpc::StateReply>(std::bind(&WithStreamedUnaryMethod_avio<BaseClass>::Streamedavio, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_avio() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status avio(::grpc::ServerContext* context, const ::transcode_rpc::avio_type* request, ::transcode_rpc::StateReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status Streamedavio(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::transcode_rpc::avio_type,::transcode_rpc::StateReply>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_process : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_process() {
      ::grpc::Service::MarkMethodStreamed(2,
        new ::grpc::internal::StreamedUnaryHandler< ::transcode_rpc::empty, ::transcode_rpc::StateReply>(std::bind(&WithStreamedUnaryMethod_process<BaseClass>::Streamedprocess, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_process() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status process(::grpc::ServerContext* context, const ::transcode_rpc::empty* request, ::transcode_rpc::StateReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status Streamedprocess(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::transcode_rpc::empty,::transcode_rpc::StateReply>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_setchannel<WithStreamedUnaryMethod_avio<WithStreamedUnaryMethod_process<Service > > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_setchannel<WithStreamedUnaryMethod_avio<WithStreamedUnaryMethod_process<Service > > > StreamedService;
};

}  // namespace transcode_rpc


#endif  // GRPC_transcode_5frpc_2eproto__INCLUDED
