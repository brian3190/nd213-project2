#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
//#define _GNU_SOURCE

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string pid = std::to_string((int)getpid());

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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
  std::ifstream stream(kProcDirectory + pid + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){   
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
      	if(key == "VmSize"){
          float value_ = std::stof(value);
          string new_value = std::to_string(value_/1000);
        	return std::stof(new_value);
        }
      };
    }
  }
  float value_ = -1; //for errors
  return value_;
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
  long int uptime_ = -1; //if error
  return uptime_;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::SysActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization() { 
  string line, cmd;
  std::ifstream stream(kProcDirectory + pid + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    return 0; //TODO
  }
  float CpuUtil = -1;
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
          return std::stoi(value) << '\n'; 
        }
      }
    }
  }
  int value_ = -1; //if error
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
  int value_ = -1; //if error
  return value_;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command() { 
  string line, cmd;
  const std::string kProcDirectory {"/proc/"};
  const std::string kCmdlineFilename {"/cmdline"};
  std::ifstream stream(kProcDirectory + pid + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, line); 
    std::istringstream linestream(line); 
    linestream >> cmd;
  }
  return cmd; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + pid + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){   
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
      	if(key == "VmSize"){
          	int value_ = std::stoi(value);
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
string LinuxParser::Uid() { 
  return "ls";
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User() { 
  string line, key, value;
  string pid = std::to_string((int)getpid());
  std::ifstream stream(kProcDirectory + pid + kStatusFilename);
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
  value = -1; //for error
  return value; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::UpTime() { 
  /*string line, utime, stime, cutime, cstime, priority, nice, num_threads, itrealvalue, starttime;
  string pid = std::to_string((int)getpid());
  std::ifstream stream(kProcDirectory + pid + kUptimeFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flag >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue >> starttime;
      return std::stol(uptime);
  }
  std::stol(uptime);*/
  string uptime = "-1";
  return uptime; 
}
