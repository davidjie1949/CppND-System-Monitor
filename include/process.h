#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command() const;             // TODO: See src/process.cpp
  float CpuUtilization() const;            // TODO: See src/process.cpp
  std::string Ram() const;                 // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(const Process &a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
 int m_pid;
 std::string m_user;
 std::string m_cmd;
 float m_cpu_utilization;
 std::string m_ram;
 long int m_up_time;
};

#endif