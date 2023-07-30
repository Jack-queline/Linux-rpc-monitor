#include "monitor_widget.h"

namespace monitor {

MonitorWidget::MonitorWidget(QWidget *parent) {}
// 显示所有监视器小部件的函数
QWidget *MonitorWidget::ShowAllMonitorWidget(const std::string& name) {  
  QWidget *widget = new QWidget();  // 创建新的 QWidget 对象
  stack_menu_ = new QStackedLayout();  // 创建新的 QStackedLayout 对象
  stack_menu_->addWidget(InitCpuMonitorWidget());  // 添加 CPU 监视器小部件
  stack_menu_->addWidget(InitSoftIrqMonitorWidget());  // 添加软中断监视器小部件
  stack_menu_->addWidget(InitMemMonitorWidget());  // 添加内存监视器小部件
  stack_menu_->addWidget(InitNetMonitorWidget());  // 添加网络监视器小部件

  QGridLayout *layout = new QGridLayout(this);  // 创建新的 QGridLayout 对象
  layout->addWidget(InitButtonMenu(name), 1, 0);  // 添加按钮菜单
  layout->addLayout(stack_menu_, 2, 0);  // 添加堆叠菜单
  widget->setLayout(layout);  // 设置布局
  return widget;  // 返回小部件
}
// 初始化按钮菜单的函数
QWidget *MonitorWidget::InitButtonMenu(const std::string& name) {  
// 创建 CPU 按钮
  QPushButton *cpu_button = new QPushButton(QString::fromStdString(name) + "_cpu", this);  
  // 创建软中断按钮
  QPushButton *soft_irq_button = new QPushButton(QString::fromStdString(name) + "_soft_irq", this);  
  // 创建内存按钮
  QPushButton *mem_button = new QPushButton(QString::fromStdString(name) + "_mem", this);  
  // 创建网络按钮
  QFont *font = new QFont("Microsoft YaHei", 15, 40);  // 创建新的 QFont 对象
  QPushButton *net_button = new QPushButton(QString::fromStdString(name) + "_net", this);  
  cpu_button->setFont(*font);  // 设置 CPU 按钮的字体
  soft_irq_button->setFont(*font);  // 设置软中断按钮的字体
  mem_button->setFont(*font);  // 设置内存按钮的字体
  net_button->setFont(*font);  // 设置网络按钮的字体

  QHBoxLayout *layout = new QHBoxLayout();  // 创建新的 QHBoxLayout 对象
  layout->addWidget(cpu_button);  // 添加 CPU 按钮
  layout->addWidget(soft_irq_button);  // 添加软中断按钮
  layout->addWidget(mem_button);  // 添加内存按钮
  layout->addWidget(net_button);  // 添加网络按钮

  QWidget *widget = new QWidget();  // 创建新的 QWidget 对象
  widget->setLayout(layout);  // 设置布局
// 连接 CPU 按钮的点击信号和槽函数
  connect(cpu_button, SIGNAL(clicked()), this, SLOT(ClickCpuButton()));  
   // 连接软中断按钮的点击信号和槽函数
  connect(soft_irq_button, SIGNAL(clicked()), this, SLOT(ClickSoftIrqButton())); 
  // 连接内存按钮的点击信号和槽函数
  connect(mem_button, SIGNAL(clicked()), this, SLOT(ClickMemButton()));  
  // 连接网络按钮的点击信号和槽函数
  connect(net_button, SIGNAL(clicked()), this, SLOT(ClickNetButton()));  

  return widget;  // 返回小部件
}

QWidget *MonitorWidget::InitCpuMonitorWidget() {  // 初始化 CPU 监视器小部件的函数
  QWidget *widget = new QWidget();  // 创建新的 QWidget 对象

  QLabel *cpu_load_label = new QLabel(this);  // 创建 CPU 负载标签
  cpu_load_label->setText(tr("Monitor CpuLoad:"));  // 设置 CPU 负载标签的文本
  cpu_load_label->setFont(QFont("Microsoft YaHei", 10, 40));  // 设置 CPU 负载标签的字体

  cpu_load_monitor_view_ = new QTableView;  // 创建新的 QTableView 对象
  cpu_load_model_ = new CpuLoadModel;  // 创建新的 CpuLoadModel 对象
  cpu_load_monitor_view_->setModel(cpu_load_model_);  // 设置 CPU 负载监视器视图的模型
  cpu_load_monitor_view_->show();  // 显示 CPU 负载监视器视图

  QLabel *cpu_stat_label = new QLabel(this);  // 创建 CPU 状态标签
  cpu_stat_label->setText(tr("Monitor CpuStat:"));  // 设置 CPU 状态标签的文本
  cpu_stat_label->setFont(QFont("Microsoft YaHei", 10, 40));  // 设置 CPU 状态标签的字体

  cpu_stat_monitor_view_ = new QTableView;  // 创建新的 QTableView 对象
  cpu_stat_model_ = new CpuStatModel;  // 创建新的 CpuStatModel 对象
  cpu_stat_monitor_view_->setModel(cpu_stat_model_);  // 设置 CPU 状态监视器视图的模型
  cpu_stat_monitor_view_->show();  // 显示 CPU 状态监视器视图

  QGridLayout *layout = new QGridLayout();  // 创建新的 QGridLayout 对象

  layout->addWidget(cpu_load_label, 3, 0);  // 添加 CPU 负载标签
  layout->addWidget(cpu_load_monitor_view_, 4, 0, 2, 2);  // 添加 CPU 负载监视器视图

  layout->addWidget(cpu_stat_label, 1, 0, 1, 1);  // 添加 CPU 状态标签
  layout->addWidget(cpu_stat_monitor_view_, 2, 0, 1, 2);  // 添加 CPU 状态监视器视图

  widget->setLayout(layout);  // 设置布局
  return widget;  // 返回小部件
}


QWidget *MonitorWidget::InitSoftIrqMonitorWidget() {  // 初始化软中断监视器小部件的函数
  QWidget *widget = new QWidget();  // 创建新的 QWidget 对象
  QLabel *monitor_label = new QLabel(this);  // 创建监视器标签
  monitor_label->setText(tr("Monitor softirq:"));  // 设置监视器标签的文本
  monitor_label->setFont(QFont("Microsoft YaHei", 10, 40));  // 设置监视器标签的字体

  monitor_view_ = new QTableView;  // 创建新的 QTableView 对象
  monitor_model_ = new MonitorBaseModel;  // 创建新的 MonitorBaseModel 对象
  // 创建新的 QSortFilterProxyModel 对象
  QSortFilterProxyModel *sort_proxy = new QSortFilterProxyModel(this);  
  sort_proxy->setSourceModel(monitor_model_);  // 设置排序代理的源模型
  monitor_view_->setModel(sort_proxy);  // 设置监视器视图的模型
  monitor_view_->setSortingEnabled(true);  // 启用监视器视图的排序功能
  monitor_view_->show();  // 显示监视器视图

  QGridLayout *layout = new QGridLayout();  // 创建新的 QGridLayout 对象
  layout->addWidget(monitor_label, 1, 0);  // 添加监视器标签
  layout->addWidget(monitor_view_, 2, 0, 1, 2);  // 添加监视器视图

  widget->setLayout(layout);  // 设置布局
  return widget;  // 返回小部件
}

QWidget *MonitorWidget::InitMemMonitorWidget() {  // 初始化内存监视器小部件的函数
  QWidget *widget = new QWidget();  // 创建新的 QWidget 对象

  QLabel *mem_label = new QLabel(this);  // 创建内存标签
  mem_label->setText(tr("Monitor mem:"));  // 设置内存标签的文本
  mem_label->setFont(QFont("Microsoft YaHei", 10, 40));  // 设置内存标签的字体

  mem_monitor_view_ = new QTableView;  // 创建新的 QTableView 对象
  mem_model_ = new MemModel;  // 创建新的 MemModel 对象
  mem_monitor_view_->setModel(mem_model_);  // 设置内存监视器视图的模型
  mem_monitor_view_->show();  // 显示内存监视器视图

  QGridLayout *layout = new QGridLayout();  // 创建新的 QGridLayout 对象

  layout->addWidget(mem_label, 1, 0);  // 添加内存标签
  layout->addWidget(mem_monitor_view_, 2, 0, 1, 1);  // 添加内存监视器视图

  widget->setLayout(layout);  // 设置布局
  return widget;  // 返回小部件
}

QWidget *MonitorWidget::InitNetMonitorWidget() {  // 初始化网络监视器小部件的函数
  QWidget *widget = new QWidget();  // 创建新的 QWidget 对象

  QLabel *net_label = new QLabel(this);  // 创建网络标签
  net_label->setText(tr("Monitor net:"));  // 设置网络标签的文本
  net_label->setFont(QFont("Microsoft YaHei", 10, 40));  // 设置网络标签的字体

  net_monitor_view_ = new QTableView;  // 创建新的 QTableView 对象
  net_model_ = new NetModel;  // 创建新的 NetModel 对象
  net_monitor_view_->setModel(net_model_);  // 设置网络监视器视图的模型
  net_monitor_view_->show();  // 显示网络监视器视图

  QGridLayout *layout = new QGridLayout();  // 创建新的 QGridLayout 对象

  layout->addWidget(net_label, 1, 0);  // 添加网络标签
  layout->addWidget(net_monitor_view_, 2, 0, 1, 1);  // 添加网络监视器视图

  widget->setLayout(layout);  // 设置布局
  return widget;  // 返回小部件
}

void MonitorWidget::UpdateData(
    const monitor::proto::MonitorInfo &monitor_info) {  // 更新数据的函数
  monitor_model_->UpdateMonitorInfo(monitor_info);  // 更新监视器模型的信息
  cpu_load_model_->UpdateMonitorInfo(monitor_info);  // 更新 CPU 负载模型的信息
  cpu_stat_model_->UpdateMonitorInfo(monitor_info);  // 更新 CPU 状态模型的信息
  mem_model_->UpdateMonitorInfo(monitor_info);  // 更新内存模型的信息
  net_model_->UpdateMonitorInfo(monitor_info);  // 更新网络模型的信息
}
// CPU 按钮点击事件的槽函数
void MonitorWidget::ClickCpuButton() { stack_menu_->setCurrentIndex(0); }  
// 软中断按钮点击事件的槽函数
void MonitorWidget::ClickSoftIrqButton() { stack_menu_->setCurrentIndex(1); }  
// 内存按钮点击事件的槽函数
void MonitorWidget::ClickMemButton() { stack_menu_->setCurrentIndex(2); }  
// 网络按钮点击事件的槽函数
void MonitorWidget::ClickNetButton() { stack_menu_->setCurrentIndex(3); }  
}  

