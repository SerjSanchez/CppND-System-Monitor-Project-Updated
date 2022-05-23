#include <string>
#include <vector>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    vector<int> cpuUsageList = LinuxParser::CpuUtilization();

    //give the positions names for readability
    int user = cpuUsageList[LinuxParser::CPUStates::kUser_];
    int nice = cpuUsageList[LinuxParser::CPUStates::kNice_];
    int system = cpuUsageList[LinuxParser::CPUStates::kSystem_]; 
    int idle = cpuUsageList[LinuxParser::CPUStates::kIdle_]; 
    int iowait = cpuUsageList[LinuxParser::CPUStates::kIOwait_]; 
    int irq = cpuUsageList[LinuxParser::CPUStates::kIRQ_]; 
    int softirq = cpuUsageList[LinuxParser::CPUStates::kSoftIRQ_]; 
    int steal = cpuUsageList[LinuxParser::CPUStates::kSteal_];

    // Apply formula
    int totalIdle = idle + iowait;
    int totalNonIdle = user + nice + system + irq + softirq + steal;

    float totalPercent = static_cast<float>(totalNonIdle) / static_cast<float>(totalNonIdle + totalIdle);

    return totalPercent;
 }