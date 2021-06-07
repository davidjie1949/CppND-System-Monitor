#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  double Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 long prev_idle;
 long prev_total;
};

#endif