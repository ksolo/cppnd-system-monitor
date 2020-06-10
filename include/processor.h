#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <fstream>
#include <string>

#define PROCESSOR_FILE_LOCATION "/proc/stat"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  ifstream data_file_(PROCESSOR_FILE_LOCATION);

  // Processor times
  float user{0.0};
  float nice{0.0};
  float system{0.0};
  float idle{0.0};
  float iowait{0.0};
  float irq{0.0};
  float softirq{0.0};
  float steal{0.0};
  float guest{0.0};
  float guest_nice{0.0};

  void ParseCPULine(std::string const &cpu_data);
};

#endif
