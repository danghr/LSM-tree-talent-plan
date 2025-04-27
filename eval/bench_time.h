#ifndef TASK1_BENCH_TIME_H_
#define TASK1_BENCH_TIME_H_

#include <iomanip>
#include <sstream>
#include <vector>

class BenchTime {
 private:
  std::vector<double> op_times = std::vector<double>();

 public:
  void add(double time);
  void clear();
  std::string statistics();
  std::string print_all();
};

#endif // TASK1_BENCH_TIME_H_
