#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  long idle = TotalIdle();
  long non_idle = TotalNonIdle();

  return float(non_idle) / float(idle + non_idle);
}

long Processor::TotalIdle() {
  std::vector<std::string> cpu_stats = LinuxParser::CpuUtilization();
  return std::stol(cpu_stats[LinuxParser::kIdle_]) +
         std::stol(cpu_stats[LinuxParser::kIOwait_]);
}
long Processor::TotalNonIdle() {
  std::vector<std::string> cpu_stats = LinuxParser::CpuUtilization();
  return std::stol(cpu_stats[LinuxParser::kUser_]) +
         std::stol(cpu_stats[LinuxParser::kNice_]) +
         std::stol(cpu_stats[LinuxParser::kSystem_]) +
         std::stol(cpu_stats[LinuxParser::kIRQ_]) +
         std::stol(cpu_stats[LinuxParser::kSoftIRQ_]) +
         std::stol(cpu_stats[LinuxParser::kSteal_]);
}
