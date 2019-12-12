#include "processor.h"
#include <fstream>
#include <sstream>
#include <string>

using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
	string s, cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  float cpu_usage = 0;
  std::ifstream stream("/proc/stat");
  if(stream.is_open()){
    std::getline(stream, s);
    std::istringstream linestream(s);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    int sum_idle = std::stoi(idle) + std::stoi(iowait);
    int sum_non_idle = std::stoi(user) + std::stoi(nice) + std::stoi(system) + std::stoi(irq) + std::stoi(softirq) + std::stoi(steal);
    int total = sum_idle + sum_non_idle;
    cpu_usage = sum_non_idle * 100 / total; //without prevIdle
  }
  return cpu_usage;
}