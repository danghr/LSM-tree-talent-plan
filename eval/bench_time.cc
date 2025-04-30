#include "bench_time.h"

#include <algorithm>

void BenchTime::add(double time) { op_times.emplace_back(time); }

void BenchTime::clear() {
  op_times.clear();
  db_stats.clear();
}

void BenchTime::add_db_stats(std::string stats) { db_stats = stats; }

std::string BenchTime::statistics() {
  if (op_times.empty()) {
    return "No operations recorded.";
  }
  std::vector<double> op_times_copy = op_times;
  double sum = 0;
  for (double time : op_times_copy) sum += time;
  double avg = sum / op_times_copy.size();

  std::sort(op_times_copy.begin(), op_times_copy.end());
  double median = op_times_copy[op_times_copy.size() / 2];
  double min = op_times_copy[0];
  double max = op_times_copy[op_times_copy.size() - 1];
  double p99 = op_times_copy[static_cast<size_t>(op_times_copy.size() * 0.99)];

  std::stringstream ss;
  ss << std::fixed << std::setprecision(3);
  ss << "Total operations: " << op_times_copy.size() << std::endl;
  ss << "Timing (us): avg=" << avg << ", median=" << median << ", min=" << min
     << ", max=" << max << ", p99=" << p99 << std::endl
     << "DB stats: " << std::endl
     << db_stats << std::endl;
  return ss.str();
}

std::string BenchTime::print_all() {
  std::stringstream ss;
  for (double time : op_times) {
    ss << time << std::endl;
  }
  return ss.str();
}