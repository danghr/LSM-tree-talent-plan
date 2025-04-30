#ifndef TASK1_BENCH_TIME_H_
#define TASK1_BENCH_TIME_H_

#include <iomanip>
#include <sstream>
#include <vector>

class BenchTime {
 private:
  std::vector<double> op_times = std::vector<double>();
  std::string db_stats;

 public:
  void add(double time);
  void clear();
  void add_db_stats(std::string stats);
  std::string statistics();
  std::string print_all();
};

#endif // TASK1_BENCH_TIME_H_
