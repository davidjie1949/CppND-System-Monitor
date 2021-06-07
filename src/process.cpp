#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) 
: m_pid(pid){
}


// TODO: Return this process's ID
int Process::Pid() { 
    return m_pid; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 
    return LinuxParser::CpuUtilization(m_pid); 
}

// TODO: Return the command that generated this process
string Process::Command() const { 
    return LinuxParser::Command(m_pid); 
}

// TODO: Return this process's memory utilization
string Process::Ram() const { 
    return LinuxParser::Ram(m_pid); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(m_pid); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(m_pid); 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator < ( const Process &a) const{
    return this->CpuUtilization() < a.CpuUtilization();
}