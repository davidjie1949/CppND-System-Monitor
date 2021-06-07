#include <string>
#include <vector>

#include "linux_parser.h"
#include "processor.h"

// TODO: Return the aggregate CPU utilization
Processor::Processor() : prev_idle(0), prev_total(0){}

double Processor::Utilization() { 
    long idle = LinuxParser::IdleJiffies();
    long total = LinuxParser::Jiffies();
    long diff_idle = idle - this->prev_idle;
    long diff_total = total - this->prev_total;
    double diff_usage = (10*(diff_total-diff_idle)/diff_total)/(double)10;
    this->prev_idle = idle;
    this->prev_total = total;
    return diff_usage;
}