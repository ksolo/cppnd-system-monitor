#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <fstream>

#define PROCESSOR_FILE_LOCATION "/proc/stat"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  ifstream data_(PROCESSOR_FILE_LOCATION);
};

#endif
