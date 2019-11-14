#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <ifstream>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() {
  return Processor& ; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
  string line, os, version, kernel;
  ifstream stream("/proc/version");
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel; //ok
  }
  return kernel; 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  string line, key, value;
  float MemUtil = -1.0; //for errors
  ifstream stream("/proc/meminfo");
  if(stream.is_open()){
    float MemTotal, MemFree;
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
      	if(key == "MemTotal"){
      		MemTotal = std::stoi(value);
        }
    	if(key == "MemFree"){
      		MemFree = std::stoi(value);
        }
      }
    }
    MemUtil = (MemTotal - MemFree) * 100 / MemTotal;	
    return MemUtil;
  }
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
  string line, key, value;
  ifstream stream("/etc/os-release");
  if(stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::replace(line.begin(), line.end(), ' ', '_');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
  string line, key, value;
  std::ifstream stream("/proc/stat");
  if(stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  value = -1; //if error
  return value;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
  string line, key, value;
  std::ifstream stream("/proc/stat");
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == "processes"){
          return std::stoi(value) << '\n'; 
        }
      }
    }
  }
  value = -1; //if error
  return value;
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
  string line, uptime;
  std::ifstream stream("/proc/uptime");
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return std::stol(uptime) << '\n';
  }
  uptime = -1; //if error
  return uptime;
}