#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

#include "linux_parser.h"

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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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
      if (std::all_of(filename.begin(), filename.end(), isdigit) && isNumber(filename)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float totalMem = 0, freeMem = 0;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string key, value;
      linestream >> key >> value;
        if (key == "MemTotal:" && isNumber(value)) {
            totalMem = stof(value);
        } else if (key == "MemFree:" && isNumber(value)) {
            freeMem = stof(value);
            break;
        }
    }
  }
  float freeMemPercent = freeMem/totalMem;
  float usedMemPercent = 1-freeMemPercent;
  return usedMemPercent;
 }

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string upTime = "";
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }
    if (isNumber(upTime)) {
      return stol(upTime);
    } else {
      return 0;
    }
}

/* 
* These functions were left uninplemented because I obtained these values 
* at the same time as other ones in other functions
*/ 
// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0.0; }

// Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<int> LinuxParser::CpuUtilization() { 
  vector<int> cpuValues;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string key, value;
      linestream >> key;
      if (key == "cpu") {
        while (linestream >> value) {
          if(isNumber(value)) {
            cpuValues.push_back(stoi(value));
          }
        }
        return cpuValues;
      }
    }
  }
  return cpuValues;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int totalProcesses;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string key, value;
      while (linestream >> key >> value) {
        if (key == "processes" && isNumber(value)) {
            totalProcesses = stoi(value);
            return totalProcesses;
        }
      }
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int runningProcesses;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string key, value;
      while (linestream >> key >> value) {
        if (key == "procs_running" && isNumber(value)) {
            runningProcesses =  stoi(value);
            return runningProcesses;
        }
      }
    }
  }
  return 0;
 }

// Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}

// Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
float LinuxParser::Ram(int pid) { 
  float usedRam;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string key, value;
      linestream >> key >> value;
        if (key == "VmRSS:" && isNumber(value)) {
            usedRam = stof(value);
            return usedRam;
        }
    }
  }
  return 0;
}

// Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string key, value;
      linestream >> key >> value;
        if (key == "Uid:") {
            return value;
        }
    }
  }
  return "";
 }

// Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid){ 
  string line;
  string username = "";
  string readUid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      string permissions;
      std::istringstream linestream(line);
      while (linestream >> username >> permissions >> readUid) {
        if (readUid == Uid(pid)) {
          return username;
        }
      }
    }
  }
  return username;
}

// Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string upTime;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; i++) {
      // 22 is the position of the uptime in the file, so the index is 21
      linestream >> upTime;
    }
  }
  if (isNumber(upTime)) {
    return LinuxParser::UpTime() - stol(upTime) / sysconf(_SC_CLK_TCK);
  } else {
    return 0;
  }
}

vector<int> LinuxParser::ProcessCpuUtilization(int pid) {
  vector<int> processCpuInfo = {};
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string value;
    for (int i = 0; i < 22; i++) {
      linestream >> value;
      if((i >= 13 && i <= 16) 
          || i == 21){
        // Positions of the cpu info in the line
        if (isNumber(value)) {
          processCpuInfo.push_back(stoi(value));
        }
      }
    }
  }
  return processCpuInfo;
}

bool LinuxParser::isNumber(const std::string& s)
{
      std::stringstream stream;                   
       double number;

       stream<<s;
       stream>>number;

       return stream.eof();
}