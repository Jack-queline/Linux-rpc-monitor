#include "monitor/cpu_softirq_monitor.h"

#include "utils/read_file.h"
#include "utils/utils.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

namespace monitor {
void CpuSoftIrqMonitor::UpdateOnce(monitor::proto::MonitorInfo* monitor_info) {
  //利用基类创建一个中断子类对象，同时打开/proc/softirqs文件
  ReadFile softirqs_file(std::string("/proc/softirqs"));
  std::vector<std::string> one_softirq;
  std::vector<std::vector<std::string>> softirq;
  //使用字符串数组one_softirq循环接收文件每一行的元素
  while (softirqs_file.ReadLine(&one_softirq)) {
    //将每一行的元素存入二维数组softirq中
    softirq.push_back(one_softirq);
    one_softirq.clear();
  }
  //对二维数组进行行列转换
  for (int i = 0; i < softirq[0].size() - 1; i++) {
    std::string name = softirq[0][i];
    //将转换后的结果存储在info中
    struct SoftIrq info;
    info.cpu_name = name;
    info.hi = std::stoll(softirq[1][i + 1]);
    info.timer = std::stoll(softirq[2][i + 1]);
    info.net_tx = std::stoll(softirq[3][i + 1]);
    info.net_rx = std::stoll(softirq[4][i + 1]);
    info.block = std::stoll(softirq[5][i + 1]);
    info.irq_poll = std::stoll(softirq[6][i + 1]);
    info.tasklet = std::stoll(softirq[7][i + 1]);
    info.sched = std::stoll(softirq[8][i + 1]);
    info.hrtimer = std::stoll(softirq[9][i + 1]);
    info.rcu = std::stoll(softirq[10][i + 1]);
    info.timepoint = boost::chrono::steady_clock::now();
    //判断存储中断信息的map当中是否已经存储数据
    auto iter = cpu_softirqs_.find(name);
    if (iter != cpu_softirqs_.end()) {
      //如果有则利用新旧数据进行计算3秒内中断产生的速率，并传给monitor_info的add_soft_irq()中
      struct SoftIrq& old = (*iter).second;
      double period = Utils::SteadyTimeSecond(info.timepoint, old.timepoint);
      auto one_softirq_msg = monitor_info->add_soft_irq();
      one_softirq_msg->set_cpu(info.cpu_name);
      one_softirq_msg->set_hi((info.hi - old.hi) / period);
      one_softirq_msg->set_timer((info.timer - old.timer) / period);
      one_softirq_msg->set_net_tx((info.net_tx - old.net_tx) / period);
      one_softirq_msg->set_net_rx((info.net_rx - old.net_rx) / period);
      one_softirq_msg->set_block((info.block - old.block) / period);
      one_softirq_msg->set_irq_poll((info.irq_poll - old.irq_poll) / period);
      one_softirq_msg->set_tasklet((info.tasklet - old.tasklet) / period);
      one_softirq_msg->set_sched((info.sched - old.sched) / period);
      one_softirq_msg->set_hrtimer((info.hrtimer - old.hrtimer) / period);
      one_softirq_msg->set_rcu((info.rcu - old.rcu) / period);
    }
    //将新的中断信息存储在map中
    cpu_softirqs_[name] = info;
  }
  return;
}

}  // namespace monitor