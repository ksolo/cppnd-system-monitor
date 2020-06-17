#include "processor.h"
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  float fraction;
  long active = LinuxParser::ActiveJiffies();
  long total = LinuxParser::Jiffies();

  fraction = float(active - previous_active) / float(total - previous_total);
  previous_active = active;
  previous_total = total;

  return fraction;
}
