#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"

class System {

 public:
 
   System(){
    //Create CPU
    cpu_ = Processor();

    //Create processes
    std::vector<int> pids = LinuxParser::Pids();
    for(int pid : pids) {
      processes_.push_back(Process(pid));
    }
    std::sort(processes_.begin(), processes_.end());
  }

  Processor& Cpu();                   // See src/system.cpp
  std::vector<Process>& Processes();  // See src/system.cpp
  float MemoryUtilization();          // See src/system.cpp
  long UpTime();                      // See src/system.cpp
  int TotalProcesses();               // See src/system.cpp
  int RunningProcesses();             // See src/system.cpp
  std::string Kernel();               // See src/system.cpp
  std::string OperatingSystem();      // See src/system.cpp

  // Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif