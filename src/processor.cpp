#include "processor.h"

#include <sstream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  if (data_file_) {
    std::string cpu_data = data_file_.getline();
    ParseCPULine(cpu_data);
  }
}

void Processor::ParseCPULine(std::string const &cpu_data) {}
