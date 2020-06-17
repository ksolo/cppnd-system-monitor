#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
	int secs = seconds % 60;
  	int minutes = (seconds / 60) % 60;
  	int hours = (seconds / 60) / 60;
  
  	string hr_string = std::to_string(hours).size() == 1 ? "0"+std::to_string(hours) : std::to_string(hours);
  	string mn_string = std::to_string(minutes).size() == 1 ? "0"+std::to_string(minutes) : std::to_string(minutes);
  	string ss_string = std::to_string(secs).size() == 1 ? "0"+std::to_string(secs) : std::to_string(secs);
  
  	return (hr_string+":"+mn_string+":"+ss_string);
}