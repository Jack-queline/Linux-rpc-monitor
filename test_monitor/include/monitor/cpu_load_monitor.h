#pragma once

#include <string>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

namespace monitor {
class CpuLoadMonitor : public MonitorInter {
 public:
  CpuLoadMonitor() {}
  //重写UpdateOnce函数，获取负载信息
  void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
  void Stop() override {}
//不同时间点的平均负载
 private:
  float load_avg_1_;
  float load_avg_3_;
  float load_avg_15_;
};

}  // namespace monitor
