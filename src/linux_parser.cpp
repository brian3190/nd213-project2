#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iterator>
//#define _GNU_SOURCE
#include "format.h"
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line, key, value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  value = "undefined"; //for errors
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    return kernel;
  }
  kernel = "undefined"; //for errors
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key, value;
  string total, free;
  string cached, SReclaimable, Shmem;
  string buffers;
  long totalUsedMem, nonCacheBufferMem, cachedMem;
  long _buffers;
  long sumMemUtil;
  float percentMemUtil;
  std::ifstream stream(kProcDirectory + kMeminfoFilename); //System memUtil
  if(stream.is_open()){
    while(std::getline(stream, line)){   
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
      	if(key == "MemTotal"){
          total = value;
        } else if(key == "MemFree"){
          free = value;
        } else if(key == "Buffers"){
          buffers = value;
        } else if(key == "Cached"){
          cached = value;
        } else if(key == "Shmem"){
          Shmem = value;
        } else if(key == "SReclaimable"){
          SReclaimable = value;
        }
      }
    }
    _buffers = std::stol(buffers);
    totalUsedMem = std::stol(total) - std::stol(free);
    cachedMem = std::stol(cached) + std::stol(SReclaimable) - std::stol(Shmem);
    nonCacheBufferMem = totalUsedMem - _buffers - cachedMem;
    sumMemUtil = nonCacheBufferMem + _buffers + cachedMem;
    percentMemUtil = ((float)sumMemUtil / std::stol(total) * 100.0); 
    return percentMemUtil; //float but displayed as int
  }
  return -1; //for errors
}

// TODO: Read and return the system uptime
long int LinuxParser::SysUpTime() { 
  string line, uptime;
  std::ifstream stream("/proc/uptime");
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return std::stol(uptime);
  }
  return -1; //for errors
}

// TODO: Read and return the number of jiffies for the system
long int LinuxParser::Jiffies [[maybe_unused]]() { 
  string value; 
  long int jiffies;
  std::ifstream stream(kProcDirectory + "$$" + kStatFilename + " | cut -d \' \' -f 22");
  if(stream.is_open()){
    std::getline(stream, value);
    jiffies = std::stol(value) * sysconf(_SC_CLK_TCK);
    return jiffies; 
  }
  return -1;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of active jiffies for the system
//long LinuxParser::SysActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
//long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) { 
  string line, uptime_;
  int uptime, utime, stime, cutime, cstime, starttime, total_time;
  float cpu_usage, seconds;
  int Hertz = (int)sysconf(_SC_CLK_TCK);
  string pid_ = std::to_string(pid);
  vector<std::string> tokens;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_;
    uptime = std::stoi(uptime_);
  }
  std::ifstream stream_(kProcDirectory + pid_ + kStatFilename);
  if(stream_.is_open()){
    std::getline(stream_, line);
    std::istringstream linestream(line);
    std::copy(std::istream_iterator<string>(linestream),
              std::istream_iterator<string>(),
              std::back_inserter(tokens));
    utime = std::stoi(tokens[13]);
    stime = std::stoi(tokens[14]);
    cutime = std::stoi(tokens[15]);
    cstime = std::stoi(tokens[16]);
    starttime = std::stoi(tokens[21]);
    total_time = utime + stime + cutime + cstime;
    seconds = uptime - (starttime / Hertz);
    cpu_usage = 100 * ((total_time / Hertz) / seconds);
    return cpu_usage;
  }
  float CpuUtil = -1.;
  return CpuUtil;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream stream("/proc/stat");
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == "processes"){
          return std::stoi(value); 
        }
      }
    }
  }
  int value_ = -1; //for errors
  return value_;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
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
  int value_ = -1; //for errors
  return value_;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line, cmd;
  string pid_ = std::to_string(pid);
  std::ifstream stream(kProcDirectory + pid_ + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, line); 
    std::istringstream linestream(line); 
    linestream >> cmd;
    return cmd;
  }
  cmd = "undefined";
  return cmd; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  string pid_ = std::to_string(pid);
  std::ifstream stream(kProcDirectory + pid_ + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){   
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
      	if(key == "VmSize"){
          long value_ = std::stof(value);
          string new_value = std::to_string(value_/1000);
        	return new_value; //string
        }
      };
    }
  }
  value = "-1";
  return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid [[maybe_unused]](int pid) { 
  string line, key, x, value;
  string pid_ = std::to_string(pid);
  std::ifstream stream(kProcDirectory + pid_ + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == "Uid"){
          return value;
        }
      }
    }
  }
  key = "Error"; // for errors
  return key;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, key, x, value, uid;
  string pid_ = std::to_string(pid);
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream, line)){    
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), '/', ' ');
      std::replace(line.begin(), line.end(), ',', ' ');
      std::istringstream linestream(line);
      linestream >> key >> x >> value;
      // Returns key of Uid(value = 1000) as first (non-root) user. Change to Uid(value = 0) for root user.
      if(value == "1000"){
        return key;
      }
    }
  }
  value = "-1"; //for error
  return value; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  long uptime = -1;
  std::vector<std::string> tokens;
  string pid_ = std::to_string(pid);
  std::ifstream stream(kProcDirectory + pid_ + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream_(line);
    //tokenize line and insert into tokens vector
    std::copy(std::istream_iterator<string>(linestream_),
              std::istream_iterator<string>(),
              std::back_inserter(tokens));
    int starttime = std::stoi(tokens[21]);
    uptime = (long)(starttime / sysconf(_SC_CLK_TCK));
    return uptime;
  }
  return uptime; 
}
