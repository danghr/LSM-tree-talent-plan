#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>
#include <chrono>
#include <cstdlib>
#include <leveldb/db.h>
#include <leveldb/status.h>
#include <leveldb/iterator.h>
#include "key_value.h"

using namespace std;

#define INSERT_KEY_COUNT 10000000
#define TEST_KEY_COUNT (INSERT_KEY_COUNT / 10)

#define LOG_FILE_NAME "timing.log"

class BenchTime {
 private:
  vector<double> op_times = vector<double>();

 public:
  void add(double time) {
    /**
     * @brief Accepts time in milliseconds
     */
    op_times.emplace_back(time);
  }
  void clear() { op_times.clear(); }
  stringstream statistics() {
    double sum = 0;
    for (double time : op_times) sum += time;
    double avg = sum / op_times.size();

    sort(op_times.begin(), op_times.end());
    double median = op_times[op_times.size() / 2];
    double min = op_times[0];
    double max = op_times[op_times.size() - 1];
    double p99 = op_times[static_cast<size_t>(op_times.size() * 0.99)];

    stringstream ss;
    ss << fixed << setprecision(3);
    ss << "Total operations: " << op_times.size() << endl;
    ss << "Timing (us): avg=" << avg << ", median=" << median << ", min=" << min
       << ", max=" << max << ", p99=" << p99;
    return ss;
  }
} bench_time;

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
  for (int i = 0; i < INSERT_KEY_COUNT; i++) {
    // Generate random key an value
    KV_Pair kv = generateRandomPair();
    string key = kv.first;
    string value = kv.second;
    // Guarantee uniqueness of keys
    // emplace returns a pair, the second element is true if the key was
    // inserted, and if a key already exists, emplace does nothing
    while (key_value_map.emplace(key, value).second == false) {
      kv = generateRandomPair();
      key = kv.first;
      value = kv.second;
    }

    // Measure time for this Put operation
    auto start = chrono::high_resolution_clock::now();
    status = (db->Put(write_options, key, value));
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> elapsed = end - start;
    bench_time.add(elapsed.count());

    if (!status.ok()) {
      cerr << "Error inserting key-value pair into database." << endl;
      cerr << status.ToString() << endl;
      delete db;
      return 1;
    }

    if ((i + 1) % 1000 == 0) {
      cout << "Inserted " << (i + 1) << " keys and values (" << fixed
           << setprecision(2)
           << (((double)(i + 1) / (double)INSERT_KEY_COUNT) * 100) << "%)"
           << endl;
    }
  }

  stringstream statistics = bench_time.statistics();
  cout << "[Stage 1] Insert " << INSERT_KEY_COUNT << " keys" << endl
       << statistics.str() << endl;
  // Write the same statistics to a file
  ofstream timing_file;
  timing_file.open(LOG_FILE_NAME, ios::out);
  timing_file << "[Stage 1] Insert " << INSERT_KEY_COUNT << " keys" << endl
              << statistics.str() << endl;

  // ---------------------------------
  // 2. Point lookup TEST_KEY_COUNT random keys and values
  // ---------------------------------
  cout << "[Stage 2] Point lookup " << TEST_KEY_COUNT << " keys..." << endl;
  bench_time.clear();
  for (int i = 0; i < TEST_KEY_COUNT; i++) {
    // Randomly select a key from the hash table
    auto it = key_value_map.begin();
    advance(it, rand() % key_value_map.size());
    string key = it->first;
    string value = it->second;
    // Measure time for this Get operation
    auto start = chrono::high_resolution_clock::now();
    string value_str;
    status = (db->Get(read_options, key, &value_str));
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> elapsed = end - start;
    bench_time.add(elapsed.count());
    if (!status.ok()) {
      cerr << "Error retrieving key-value pair from database." << endl;
      cerr << status.ToString() << endl;
      delete db;
      timing_file.close();
      return 1;
    }
    // Check if the retrieved value matches the original value
    if (value_str != value) {
      cerr << "Error: Retrieved value does not match original value." << endl;
      cerr << "Key: \"" << key << "\"" << endl;
      cerr << "Original Value: \"" << value << "\"" << endl;
      cerr << "Retrieved Value: \"" << value_str << "\"" << endl;
      delete db;
      timing_file.close();
      return 1;
    }
    if ((i + 1) % 1000 == 0) {
      cout << "Retrieved " << (i + 1) << " keys (" << fixed << setprecision(2)
           << (((double)(i + 1) / (double)TEST_KEY_COUNT) * 100) << "%)"
           << endl;
    }
  }
  statistics = bench_time.statistics();
  cout << "[Stage 2] Point lookup " << TEST_KEY_COUNT << " keys" << endl
       << statistics.str() << endl;
  timing_file << "[Stage 2] Point lookup " << TEST_KEY_COUNT << " keys" << endl
              << statistics.str() << endl;

  // ---------------------------------
  // 3. Sequentially delete TEST_KEY_COUNT keys
  // ---------------------------------
  cout << "[Stage 3] Sequentially deleting " << TEST_KEY_COUNT << " keys..."
       << endl;
  bench_time.clear();
  for (int i = 0; i < TEST_KEY_COUNT; i++) {
    // Randomly select a key from the hash table
    auto it = key_value_map.begin();
    advance(it, rand() % key_value_map.size());
    string key = it->first;
    // Measure time for this Delete operation
    auto start = chrono::high_resolution_clock::now();
    status = (db->Delete(write_options, key));
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> elapsed = end - start;
    bench_time.add(elapsed.count());
    if (!status.ok()) {
      cerr << "Error deleting key-value pair from database." << endl;
      cerr << status.ToString() << endl;
      delete db;
      timing_file.close();
      return 1;
    }
    // Remove the key from the hash table
    key_value_map.erase(key);
    if ((i + 1) % 1000 == 0) {
      cout << "Deleted " << (i + 1) << " keys (" << fixed << setprecision(2)
           << (((double)(i + 1) / (double)TEST_KEY_COUNT) * 100) << "%)"
           << endl;
    }
  }

  statistics = bench_time.statistics();
  cout << "[Stage 3] Sequentially delete " << TEST_KEY_COUNT << " keys" << endl
       << statistics.str() << endl;
  timing_file << "[Stage 3] Sequentially delete " << TEST_KEY_COUNT << " keys" << endl
              << statistics.str() << endl;
    
  // ---------------------------------
  // 4. Sequentially update TEST_KEY_COUNT keys
  // ---------------------------------

  cout << "[Stage 4] Sequentially updating " << TEST_KEY_COUNT << " keys..."
       << endl;
  bench_time.clear();
  for (int i = 0; i < TEST_KEY_COUNT; i++) {
    // Randomly select a key from the hash table
    auto it = key_value_map.begin();
    advance(it, rand() % key_value_map.size());
    string key = it->first;
    string value = it->second;
    // Generate a new random value
    KV_Pair kv = generateRandomPair();
    string new_value = kv.second;

    // Measure time for this Put operation
    auto start = chrono::high_resolution_clock::now();
    status = (db->Put(write_options, key, new_value));
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> elapsed = end - start;
    bench_time.add(elapsed.count());
    if (!status.ok()) {
      cerr << "Error updating key-value pair in database." << endl;
      cerr << status.ToString() << endl;
      delete db;
      timing_file.close();
      return 1;
    }
    // Update the value in the hash table
    key_value_map[key] = new_value;

    if ((i + 1) % 1000 == 0) {
      cout << "Updated " << (i + 1) << " keys (" << fixed << setprecision(2)
           << (((double)(i + 1) / (double)TEST_KEY_COUNT) * 100) << "%)"
           << endl;
    }
  }

  statistics = bench_time.statistics();
  cout << "[Stage 4] Sequentially update " << TEST_KEY_COUNT << " keys" << endl
       << statistics.str() << endl;
  timing_file << "[Stage 4] Sequentially update " << TEST_KEY_COUNT << " keys" << endl
              << statistics.str() << endl;  
  
  // ---------------------------------
  // 5. Range lookup TEST_KEY_COUNT keys
  // ---------------------------------
  cout << "[Stage 5] Range lookup " << TEST_KEY_COUNT << " keys..." << endl;
  bench_time.clear();
  for (int i = 0; i < TEST_KEY_COUNT; i++) {
    // Randomly select a key from the hash table
    auto it = key_value_map.begin();
    advance(it, rand() % key_value_map.size());
    string key = it->first;
    string value = it->second;
    // Measure time for this Get operation
    leveldb::Iterator* dbit = db->NewIterator(leveldb::ReadOptions());
    vector<string> keys;
    vector<string> values;
    int cnt = 0;
    auto start = chrono::high_resolution_clock::now();
    for (dbit->Seek(key); dbit->Valid() && cnt < 16; dbit->Next()) {
      keys.push_back(dbit->key().ToString());
      values.push_back(dbit->value().ToString());
      cnt++;
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> elapsed = end - start;
    bench_time.add(elapsed.count());
    // Check correctness of results
    for (int i = 0; i < keys.size(); i++) {
      if (values[i] != key_value_map[keys[i]]) {
        cerr << "Error: Retrieved value does not match original value." << endl;
        cerr << "Key: \"" << keys[i] << "\"" << endl;
        cerr << "Original Value: \"" << key_value_map[keys[i]] << "\"" << endl;
        cerr << "Retrieved Value: \"" << values[i] << "\"" << endl;
        delete dbit;
        delete db;
        timing_file.close();
        return 1;
      }
    }

    if ((i + 1) % 1000 == 0) {
      cout << "Updated " << (i + 1) << " keys (" << fixed << setprecision(2)
           << (((double)(i + 1) / (double)TEST_KEY_COUNT) * 100) << "%)"
           << endl;
    }
  }

  statistics = bench_time.statistics();
  cout << "[Stage 5] Range lookup " << TEST_KEY_COUNT << " keys" << endl
       << statistics.str() << endl;
  timing_file << "[Stage 5] Range lookup " << TEST_KEY_COUNT << " keys" << endl
              << statistics.str() << endl;

  // Clean up
  delete db;  // Close the database
  timing_file.close();

  return 0;
}
