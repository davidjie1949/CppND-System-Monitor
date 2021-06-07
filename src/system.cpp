#include <unistd.h>
#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

// using std::map;
// using std::set;
// using std::size_t;
// using std::string;
// using std::vector;
using namespace std;

System::System(){
    cpu_ = Processor();
}

// Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; 
}

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    refreshProcessesList();
    std::sort(processes_.begin(), processes_.end());
    std::reverse(std::begin(processes_), std::end(processes_));
    return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() const { 
    return LinuxParser::Kernel(); 
}

// Return the system's memory utilization
float System::MemoryUtilization() const { 
    return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem() const { 
    return LinuxParser::OperatingSystem(); 
}

// Return the number of processes actively running on the system
int System::RunningProcesses() const { 
    return LinuxParser::RunningProcesses(); 
}

// Return the total number of processes on the system
int System::TotalProcesses() const { 
    return LinuxParser::TotalProcesses(); 
}

// Return the number of seconds since the system started running
long System::UpTime() const { 
    return LinuxParser::UpTime(); 
}

void System::refreshProcessesList(){
    processes_.clear();
    vector<int> pids = LinuxParser::Pids();
    for(int pid : pids){
        processes_.emplace_back(Process(pid));
    }
}