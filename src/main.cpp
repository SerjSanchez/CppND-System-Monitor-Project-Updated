#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"

int main() {
  System system;
  //NCursesDisplay::Display(system);
  LinuxParser::UpTime();
  LinuxParser::MemoryUtilization();
  LinuxParser::UpTime();
  LinuxParser::CpuUtilization();
  LinuxParser::TotalProcesses();
  LinuxParser::RunningProcesses();

  std::vector<int> pids = LinuxParser::Pids();
  LinuxParser::Command(pids[2]);
  LinuxParser::Ram(pids[2]);
  int uid = LinuxParser::Uid(pids[2]);
  LinuxParser::User(pids[2], uid);
  LinuxParser::UpTime(pids[2]);
}