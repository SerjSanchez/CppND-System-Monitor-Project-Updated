#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
    Process(int pid) 
    : pid_(pid)
    {
        calculateCpuUtilization();
    }

  int Pid();                               
  std::string User();            
  std::string Command();                
  float CpuUtilization();            
  std::string Ram();                      
  long int UpTime();
  bool operator<(Process const& a) const;  
  void calculateCpuUtilization();

  // Declare any necessary private members
 private:
    int pid_ = 0;
    float cpuUtilization_ = 0;
};

#endif