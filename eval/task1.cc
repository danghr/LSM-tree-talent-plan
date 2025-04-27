#include <cstdlib>
#include <fstream>
#include <iostream>
#include <leveldb/db.h>
#include <leveldb/iterator.h>
#include <leveldb/status.h>
#include <map>

#include "bench_time.h"
#include "key_value.h"
#include "operation.h"

using namespace std;

#define INSERT_KEY_COUNT 10000
#define TEST_KEY_COUNT (INSERT_KEY_COUNT / 10)

#define LOG_FILE_NAME "timing"

BenchTime bench_time;

int main() {
  // Remove the old database
  system("rm -rf db");

  // Create and open a new database
  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::WriteOptions write_options;
  // write_options.sync = true;
  leveldb::ReadOptions read_options;
  leveldb::Status status = leveldb::DB::Open(options, "db", &db);
  if (!status.ok()) {
    cerr << "Unable to open/create test database." << endl;
    cerr << status.ToString() << endl;
    return 1;
  }
  cout << "Database opened successfully." << endl;

  // Create a hash table to store the keys and values
  map<string, string> key_value_map;

  // Vector to store operation times (microseconds)
  vector<double> op_times;

  // ---------------------------------
  // 1. Insert INSERT_KEY_COUNT random keys and values
  // ---------------------------------
  cout << "[Stage 1] Inserting " << INSERT_KEY_COUNT
       << "random keys and values..." << endl;

  BenchTime bench_time =
      fillrandom(db, write_options, key_value_map, INSERT_KEY_COUNT);

  char stage_1_file_name[100] = LOG_FILE_NAME;
  strcat(stage_1_file_name, "-stage_1.log");
  ofstream stage_1_file;
  stage_1_file.open(stage_1_file_name, ios::out);
  stage_1_file << bench_time.print_all();
  stage_1_file.close();

  string statistics = bench_time.statistics();
  cout << "[Stage 1] Insert " << INSERT_KEY_COUNT << " keys" << endl
       << statistics << endl;
  // Write the same statistics to a file
  char timing_file_name[100] = LOG_FILE_NAME;
  strcat(timing_file_name, ".log");
  ofstream timing_file;
  timing_file.open(timing_file_name, ios::out);
  timing_file << "[Stage 1] Insert " << INSERT_KEY_COUNT << " keys" << endl
              << statistics << endl
              << endl;

  // ---------------------------------
  // 2. Point lookup TEST_KEY_COUNT random keys and values
  // ---------------------------------
  cout << "[Stage 2] Point lookup " << TEST_KEY_COUNT << " keys..." << endl;

  bench_time = readrandom(db, read_options, key_value_map, TEST_KEY_COUNT);

  char stage_2_file_name[100] = LOG_FILE_NAME;
  strcat(stage_2_file_name, "-stage_2.log");
  ofstream stage_2_file;
  stage_2_file.open(stage_2_file_name, ios::out);
  stage_2_file << bench_time.print_all();
  stage_2_file.close();

  statistics = bench_time.statistics();
  cout << "[Stage 2] Point lookup " << TEST_KEY_COUNT << " keys" << endl
       << statistics << endl;
  timing_file << "[Stage 2] Point lookup " << TEST_KEY_COUNT << " keys" << endl
              << statistics << endl
              << endl;

  // ---------------------------------
  // 3. Sequentially delete TEST_KEY_COUNT keys
  // ---------------------------------
  cout << "[Stage 3] Sequentially deleting " << TEST_KEY_COUNT << " keys..."
       << endl;

  bench_time = delrandom(db, write_options, key_value_map, TEST_KEY_COUNT);

  char stage_3_file_name[100] = LOG_FILE_NAME;
  strcat(stage_3_file_name, "-stage_3.log");
  ofstream stage_3_file;
  stage_3_file.open(stage_3_file_name, ios::out);
  stage_3_file << bench_time.print_all();
  stage_3_file.close();

  statistics = bench_time.statistics();
  cout << "[Stage 3] Sequentially delete " << TEST_KEY_COUNT << " keys" << endl
       << statistics << endl;
  timing_file << "[Stage 3] Sequentially delete " << TEST_KEY_COUNT << " keys"
              << endl
              << statistics << endl
              << endl;

  // ---------------------------------
  // 4. Sequentially update TEST_KEY_COUNT keys
  // ---------------------------------

  cout << "[Stage 4] Sequentially updating " << TEST_KEY_COUNT << " keys..."
       << endl;

  bench_time = updaterandom(db, write_options, key_value_map, TEST_KEY_COUNT);

  char stage_4_file_name[100] = LOG_FILE_NAME;
  strcat(stage_4_file_name, "-stage_4.log");
  ofstream stage_4_file;
  stage_4_file.open(stage_4_file_name, ios::out);
  stage_4_file << bench_time.print_all();
  stage_4_file.close();

  statistics = bench_time.statistics();
  cout << "[Stage 4] Sequentially update " << TEST_KEY_COUNT << " keys" << endl
       << statistics << endl;
  timing_file << "[Stage 4] Sequentially update " << TEST_KEY_COUNT << " keys"
              << endl
              << statistics << endl
              << endl;

  // ---------------------------------
  // 5. Range lookup TEST_KEY_COUNT keys
  // ---------------------------------
  cout << "[Stage 5] Range lookup " << TEST_KEY_COUNT << " keys..." << endl;

  bench_time = rangequery(db, read_options, key_value_map, TEST_KEY_COUNT);

  char stage_5_file_name[100] = LOG_FILE_NAME;
  strcat(stage_5_file_name, "-stage_5.log");
  ofstream stage_5_file;
  stage_5_file.open(stage_5_file_name, ios::out);
  stage_5_file << bench_time.print_all();
  stage_5_file.close();

  statistics = bench_time.statistics();
  cout << "[Stage 5] Range lookup " << TEST_KEY_COUNT << " keys" << endl
       << statistics << endl;
  timing_file << "[Stage 5] Range lookup " << TEST_KEY_COUNT << " keys" << endl
              << statistics << endl
              << endl;

  // Clean up
  delete db;  // Close the database
  timing_file.close();

  return 0;
}
