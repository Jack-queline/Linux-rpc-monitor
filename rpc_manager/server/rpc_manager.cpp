#include "rpc_manager.h"
#include <iostream>

namespace monitor {
GrpcManagerImpl::GrpcManagerImpl() {}

GrpcManagerImpl::~GrpcManagerImpl() {}
::grpc::Status GrpcManagerImpl::SetMonitorInfo(::grpc::ServerContext* context,const ::monitor::proto::MonitorInfo* request,
::google::protobuf::Empty* response) {
  monitor_infos_.Clear();
  monitor_infos_ = *request;
  return grpc::Status::OK;
}
::grpc::Status GrpcManagerImpl::GetMonitorInfo(
    ::grpc::ServerContext* context, const ::google::protobuf::Empty* request,
    ::monitor::proto::MonitorInfo* response) {

  *response = monitor_infos_;
  return grpc::Status::OK;
}

}  // namespace monitor
