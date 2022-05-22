#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

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
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
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
  float totalMem, freeMem;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string key, value;
      linestream >> key >> value;
        if (key == "MemTotal:") {
            //std::cout << "Total mem: " << key << " " << value << "\n";// TODO remove when finished
            totalMem = stof(value);
        } else if (key == "MemFree:") {
            //std::cout  << "Free mem: "<< key << " " << value << "\n";// TODO remove when finished
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
  string upTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }
  //std::cout << "Uptime: " << upTime << " " << upTime << "\n";// TODO remove when finished
  return stol(upTime);
}

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
          //std::cout << "CPU usage: " << key << " " << value << "\n";// TODO remove when finished
          cpuValues.push_back(stoi(value));
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
        if (key == "processes") {
            //std::cout << "Total process count: " << key << " " << value << "\n";// TODO remove when finished
            totalProcesses = stoi(value);
            return totalProcesses;
        }
      }
    }
  }
  return totalProcesses;
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
        if (key == "procs_running") {
            //std::cout << "Running process count: " << key << " " << value << "\n";// TODO remove when finished
            runningProcesses =  stoi(value);
            return runningProcesses;
        }
      }
    }
  }
  return runningProcesses;
 }

// Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  //std::cout << "Process command: " << line << "\n"; // TODO remove when finished
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
        if (key == "VmSize:") {
            //std::cout << "Process RAM: " << key << " " << value << "\n";// TODO remove when finished
            usedRam = stof(value);
            return usedRam;
        }
    }
  }
  return usedRam;
}

// Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
int LinuxParser::Uid(int pid) { 
  int uid;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string key, value;
      linestream >> key >> value;
        if (key == "Uid:") {
            //std::cout << "Process Uid: " << key << " " << value << "\n";// TODO remove when finished
            uid = stoi(value);
            return uid;
        }
    }
  }
  return uid;
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int uid){ 
  string line;
  string username;
  string readUid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      string permissions;
      std::istringstream linestream(line);
      while (linestream >> username >> permissions >> readUid) {
        if (stoi(readUid) == uid) {
          //std::cout << "Process username: " << username << " " << readUid << "\n"; // TODO remove when finished
          return username;
        }
      }
    }
  }
  return username;
}

// TODO: Read and return the uptime of a process
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
  //std::cout << "Process uptime: " << upTime << "\n"; // TODO remove when finished
  return stol(upTime);
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
        processCpuInfo.push_back(stoi(value));
        //std::cout << "Process cpu: " << value << "\n"; // TODO remove when finished
      }
    }
  }
  return processCpuInfo;
}
