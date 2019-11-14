#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { 
	return (int)getpid();
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
	string line,   
}

// TODO: Return the command that generated this process
string Process::Command() { 
  string line, cmd;
  string pid = std::to_string((int)getpid());
  std::ifstream stream(kProcDirectory + pid + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, line); 
    std::istringstream linestream(line); 
    linestream >> cmd;
  }
  return cmd;
}

// TODO: Return this process's memory utilization
string Process::Ram() {
  string line, key, value;
  string pid = std::to_string(getpid());
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
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
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

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  string line, key, comm, state, ppid, pgrp, session, tty_nr, tpgid, flag, minflt, cminflt, majflt, cmajflt, utime, stime, cutime, cstime, priority, nice, num_threads, itrealvalue, starttime;
  string pid = std::to_string((int)getpid());
  std::ifstream stream(kProcDirectory + pid + kUptimeFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flag >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue >> starttime;
      std::cout
  }
  std::stol(uptime);
  return uptime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }