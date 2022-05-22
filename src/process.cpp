#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;


// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    float ramKB = LinuxParser::Ram(pid_);
    // to string with 2 decimals
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << ramKB/1024.0;
    std::string ramMB = stream.str();    
    return ramMB;
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    int uid = LinuxParser::Uid(pid_);
    return LinuxParser::User(uid);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return this->cpuUtilization_ < a.cpuUtilization_;
}