#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::atoi;
using std::atol;
using std::string;
using std::to_string;
using std::vector;
using std::stoi;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line, key, value;
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
  return "Linux";
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
  stream.close();
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string mem_total, mem_free, mem_available, buffers; 
  string title, line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open()) {

    for (int i = 0; i < 4; i++){
      string value;
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> title >> value;

      if (title == "MemTotal:") {
          mem_total = value;
          if(mem_total == ""){
            return 0;
          }
        }else if (title == "MemFree:"){
          mem_free = value;
          if(mem_free == ""){
            return 0;
          }
        }else if (title == "MemAvailable:"){
          mem_available = value;
          if(mem_available == ""){
            return 0;
          }
        }else if (title == "Buffers:"){
          buffers = value;
          if(buffers == ""){
            return 0;
          }
        }
    }
  }

  float memUtil = (stof(mem_total.c_str()) - stof(mem_free.c_str())) / stof(mem_total.c_str());
  return memUtil;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string upTime, waitTime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> waitTime;
  }

  return atol(upTime.c_str());
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
    vector<string> jiffies = CpuUtilization();
  long total_jiffies_since_boot = 0;
  for(string s : jiffies){
    total_jiffies_since_boot += atol(s.c_str());
  }
  return total_jiffies_since_boot; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { 
//   return 0;
// }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffies = CpuUtilization();
  long idle_jiffies_since_boot = atol(jiffies[3].c_str());

  return idle_jiffies_since_boot; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> v{};
  string user, nice, sys, idle, iowait, irq, softirq, steal;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> sys >> idle >> iowait >> irq >> softirq >> steal;
  }
    v.emplace_back(user);
    v.emplace_back(nice);
    v.emplace_back(sys);
    v.emplace_back(idle);
    v.emplace_back(iowait);
    v.emplace_back(irq);
    v.emplace_back(softirq);
    v.emplace_back(steal);

  return v; 
}

float LinuxParser::CpuUtilization(int pid) {
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  std::getline(stream, line);
  std::istringstream linestream(line);
  std::istream_iterator<string> beg(linestream), end;
  vector<string> values(beg, end);

  float hertz = sysconf(_SC_CLK_TCK);
  float utime = stof(values[13].c_str());
  float stime = stof(values[14].c_str());
  float cutime = stof(values[15].c_str());
  float cstime = stof(values[16].c_str());
  float starttime = stof(values[21].c_str()) / hertz;
  
  float uptime = UpTime();  // sys uptime
  
  float total_time = float(utime + stime + cutime + cstime) / hertz;
  float seconds = uptime - starttime;
  return total_time / seconds;
}
  
// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string title, totalProcess;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
    std::istringstream linestream(line);
    linestream >> title >> totalProcess;
        if (title == "processes") {
            break;
        }
    }
  }
  
  if(totalProcess == ""){
    return 0;
  }
  return stoi(totalProcess.c_str());
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string title, runningProcess;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
    std::istringstream linestream(line);
    linestream >> title >> runningProcess;
        if (title == "procs_running") {
            break;
        }
    }
  }
  if(runningProcess == ""){
    return 0;
  }
  return stoi(runningProcess.c_str());
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line, cmd;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    for (int i = 0; i < 50; i++){
        cmd = cmd + line[i];
    }
  }
  return cmd; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string name = "VmData";
  string title, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  while (std::getline(stream, line)){
    if(line.compare(0, name.size(), name) == 0){
      std::istringstream linestream(line);
      linestream >> title >> value;
      break;
    }
  }
  if (value == ""){
    return "0";
  }

  value = to_string(stof(value.c_str())/1024);
  return value.substr(0,6); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string name = "Uid";
  string title, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  while (std::getline(stream, line)){
    if(line.compare(0, name.size(), name) == 0){
      std::istringstream linestream(line);
      linestream >> title >> value;
      break;
    }
  }
  return value; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string name = "x:" + Uid(pid);
  string title, value;
  std::ifstream stream(kPasswordPath);
  string line;
  while (std::getline(stream, line)){
    if(line.find(name) != std::string::npos){
      value = line.substr(0, line.find(":"));
    }
  }
  return value; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  std::getline(stream, line);
  std::istringstream linestream(line);
  std::istream_iterator<string> beg(linestream), end;
  vector<string> values(beg, end);

  float seconds = float(stof(values[21].c_str()) / sysconf(_SC_CLK_TCK));
  return seconds; 
}