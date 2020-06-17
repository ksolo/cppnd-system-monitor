#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

// debugging
#include <cassert>
#include <iostream>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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
	std::ifstream mem_info_file(kProcDirectory + kMeminfoFilename);
  	if (mem_info_file) {
      float mem_total{0.0}, mem_free{0.0};
      while(mem_info_file.is_open()) {
      	string mem_line;
        getline(mem_info_file, mem_line);
        std::istringstream mem_stream(mem_line);
        
        string key, value;
        mem_stream >> key >> value;
        if (key == "MemTotal:") { mem_total = std::stof(value); }
        if (key == "MemFree:") { mem_free = std::stof(value); }
        if (mem_total && mem_free) { 
          mem_info_file.close();
          return (mem_total - mem_free) / mem_total;
        }
      }
      mem_info_file.close();
    }
  	return float{0};
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream uptime_file(kProcDirectory + kUptimeFilename);
  if (uptime_file) {
  	string uptime_line;
    getline(uptime_file, uptime_line);
    uptime_file.close();
    
    long uptime;
    std::istringstream uptime_stream(uptime_line);
    uptime_stream >> uptime;
    return uptime;
  }
  return long{0};
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
	vector<string> pid_stats = CpuUtilization(pid);
  	return std::stol(pid_stats[13]) + std::stol(pid_stats[14]) + std::stol(pid_stats[15]) + std::stol(pid_stats[16]);
}

vector<string> LinuxParser::CpuUtilization(int pid) {
	std::ifstream pid_stat_file(kProcDirectory + std::to_string(pid) + kStatFilename);
  	if (pid_stat_file) {
      string stat_line;
      getline(pid_stat_file, stat_line);
      pid_stat_file.close();
      
      std::istringstream stat_stream(stat_line);
      std::vector<string> tokens;
      string token;
      
      while(stat_stream >> token) {
      	tokens.push_back(token);
      }
      return tokens;
    }
  	return {};
}


// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::vector<std::string> cpu_stats = CpuUtilization();
  return std::stol(cpu_stats[kUser_]) + std::stol(cpu_stats[kNice_]) +
         std::stol(cpu_stats[kSystem_]) + std::stol(cpu_stats[kIRQ_]) +
         std::stol(cpu_stats[kSoftIRQ_]) + std::stol(cpu_stats[kSteal_]);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::vector<std::string> cpu_stats = CpuUtilization();
  return std::stol(cpu_stats[kIdle_]) + std::stol(cpu_stats[kIOwait_]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> results;
  std::ifstream proc_stat(kProcDirectory + kStatFilename);
  if (proc_stat) {
    string cpu_line;
    getline(proc_stat, cpu_line);
    proc_stat.close();

    string token;
    std::istringstream cpu_stats(cpu_line);
    while (cpu_stats >> token) {
      if (token == "cpu") {
        continue;
      }
      results.push_back(token);
    }
  }
  return results;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
	std::ifstream proc_stat_file(kProcDirectory + kStatFilename);
  	if (proc_stat_file) {
      while(proc_stat_file.is_open()) {
    	string stat_line;
        getline(proc_stat_file, stat_line);
      	
      	string key, value;
        std::istringstream stat_line_stream(stat_line);
        stat_line_stream >> key >> value;
        if (key == "processes") {
          	proc_stat_file.close();
        	return std::stoi(value);
        }
      }
    }
  	return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
	std::ifstream proc_stat_file(kProcDirectory + kStatFilename);
  	if (proc_stat_file) {
      while(proc_stat_file.is_open()) {
    	string stat_line;
        getline(proc_stat_file, stat_line);
      	
      	string key, value;
        std::istringstream stat_line_stream(stat_line);
        stat_line_stream >> key >> value;
        if (key == "procs_running") {
          	proc_stat_file.close();
        	return std::stoi(value);
        }
      }
    }
  	return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::ifstream command_stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (command_stream) {
	string command_line;
    getline(command_stream, command_line);
    command_stream.close();
    return command_line;
  }
  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
	std::ifstream proc_status_file(kProcDirectory + std::to_string(pid) + kStatusFilename);
  	if (proc_status_file) {
    	while (proc_status_file.is_open()) {
        	string proc_line;
          	getline(proc_status_file, proc_line);
          	std::istringstream proc_stream(proc_line);
          
          	string key, value;
          	proc_stream >> key >> value;
          	if (key == "VmSize:") {
            	proc_status_file.close();
              	long mem = std::stol(value);
              	return std::to_string(mem / 1024);
            }
        }
    }
  	return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::ifstream status_file(kProcDirectory + std::to_string(pid) + kStatusFilename);
  string uid;
  if (status_file) {
    while(status_file.is_open()) {
      string status_line;
      getline(status_file, status_line);
      std::istringstream status_stream(status_line);
      
      string key, value;
      status_stream >> key >> value;
      if (key == "Uid:") {
        uid = value;
        status_file.close();
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  std::ifstream etc_password(kPasswordPath);
  if (etc_password) {
    while(etc_password.is_open()) {
      string line;
      getline(etc_password, line);

      std::istringstream line_stream(line);
      string user, mode, matching_uid;
      getline(line_stream, user, ':');
      getline(line_stream, mode, ':');
      getline(line_stream, matching_uid, ':');

      if (matching_uid == uid) {
      	etc_password.close();
        return user;
      }
    }
    etc_password.close();
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
	std::vector<std::string> pid_stat = CpuUtilization(pid);
  	return UpTime() - (std::stol(pid_stat[21]) / sysconf(_SC_CLK_TCK)) ;
}
