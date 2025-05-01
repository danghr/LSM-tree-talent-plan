#include "operation.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

#include "bench_time.h"
#include "key_value.h"

using namespace std;

static void add_db_stats(leveldb::DB* db, BenchTime& bench_time) {
  leveldb::Options options;
  string stats;
  db->GetProperty("leveldb.stats", &stats);
  bench_time.add_db_stats(stats);
}

BenchTime fillrandom(leveldb::DB* db, leveldb::WriteOptions& write_options,
                     map<string, string>& key_value_map, int count) {
  BenchTime bench_time;
  leveldb::Status status;
  for (int i = 0; i < count; i++) {
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
      exit(1);
    }

    if ((i + 1) % 1000 == 0) {
      cout << "Inserted " << (i + 1) << " keys and values (" << fixed
           << setprecision(2) << (((double)(i + 1) / (double)count) * 100)
           << "%)" << endl;
    }
  }
  add_db_stats(db, bench_time);
  return bench_time;
}

BenchTime readrandom(leveldb::DB* db, leveldb::ReadOptions& read_options,
                     map<string, string>& key_value_map, int count) {
  BenchTime bench_time;
  leveldb::Status status;
  for (int i = 0; i < count; i++) {
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
      exit(1);
    }
    // Check if the retrieved value matches the original value
    if (value_str != value) {
      cerr << "Error: Retrieved value does not match original value." << endl;
      cerr << "Key: \"" << key << "\"" << endl;
      cerr << "Original Value: \"" << value << "\"" << endl;
      cerr << "Retrieved Value: \"" << value_str << "\"" << endl;
      delete db;
      exit(1);
    }
    if ((i + 1) % 1000 == 0) {
      cout << "Point lookup " << (i + 1) << " keys (" << fixed
           << setprecision(2) << (((double)(i + 1) / (double)count) * 100)
           << "%)" << endl;
    }
  }
  add_db_stats(db, bench_time);
  return bench_time;
}

BenchTime delrandom(leveldb::DB* db, leveldb::WriteOptions& write_options,
                    leveldb::ReadOptions& read_options,
                    map<string, string>& key_value_map, int count) {
  BenchTime bench_time;
  leveldb::Status status;
  vector<string> deleted_keys;
  for (int i = 0; i < count; i++) {
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
      exit(1);
    }
    // Remove the key from the hash table
    key_value_map.erase(key);
    deleted_keys.emplace_back(key);
    if ((i + 1) % 1000 == 0) {
      cout << "Deleted " << (i + 1) << " keys (" << fixed << setprecision(2)
           << (((double)(i + 1) / (double)count) * 100) << "%)" << endl;
    }
  }
  // Check correctness of results
  cout << "Checking correctness of deleted keys..." << endl;
  for (const auto& key : deleted_keys) {
    string value_str;
    status = (db->Get(read_options, key, &value_str));
    if (status.ok()) {
      cerr << "Error: Retrieved value for deleted key." << endl;
      cerr << "Key: \"" << key << "\"" << endl;
      cerr << "Retrieved Value: \"" << value_str << "\"" << endl;
      delete db;
      exit(1);
    }
  }
  add_db_stats(db, bench_time);
  return bench_time;
}

BenchTime updaterandom(leveldb::DB* db, leveldb::WriteOptions& write_options,
                       leveldb::ReadOptions& read_options,
                       std::map<std::string, std::string>& key_value_map,
                       int count) {
  BenchTime bench_time;
  leveldb::Status status;
  vector<string> updated_keys;
  for (int i = 0; i < count; i++) {
    // Randomly select a key from the hash table
    auto it = key_value_map.begin();
    advance(it, rand() % key_value_map.size());
    string key = it->first;
    string value = it->second;
    // Generate a new random value
    KV_Pair kv = generateRandomPair();
    string new_value = kv.second;

    updated_keys.emplace_back(key);

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
      exit(-1);
    }
    // Update the value in the hash table
    key_value_map[key] = new_value;

    if ((i + 1) % 1000 == 0) {
      cout << "Updated " << (i + 1) << " keys (" << fixed << setprecision(2)
           << (((double)(i + 1) / (double)count) * 100) << "%)" << endl;
    }
  }
  add_db_stats(db, bench_time);

  // Check correctness of results
  cout << "Checking correctness of updated keys..." << endl;
  for (const auto& key : updated_keys) {
    string value_str;
    status = (db->Get(read_options, key, &value_str));
    if (!status.ok()) {
      cerr << "Error retrieving key-value pair from database." << endl;
      cerr << status.ToString() << endl;
      delete db;
      exit(-1);
    }
    // Check if the retrieved value matches the updated value
    if (value_str != key_value_map[key]) {
      cerr << "Error: Retrieved value does not match updated value." << endl;
      cerr << "Key: \"" << key << "\"" << endl;
      cerr << "Updated Value: \"" << key_value_map[key] << "\"" << endl;
      cerr << "Retrieved Value: \"" << value_str << "\"" << endl;
      delete db;
      exit(-1);
    }
  }
  cout << "Correctness check completed." << endl;
  return bench_time;
}

BenchTime rangequery(leveldb::DB* db, leveldb::ReadOptions& read_options,
                     std::map<std::string, std::string>& key_value_map,
                     int count) {
  BenchTime bench_time;
  leveldb::Status status;
  for (int i = 0; i < count; i++) {
    // Randomly select a key from the hash table
    auto it = key_value_map.begin();
    advance(it, rand() % key_value_map.size());
    string key = it->first;
    string value = it->second;
    // Measure time for this Get operation
    leveldb::Iterator* dbit = db->NewIterator(read_options);
    vector<string> keys;
    vector<string> values;
    int cnt = 0;
    auto start = chrono::high_resolution_clock::now();
    for (dbit->Seek(key); dbit->Valid() && cnt < 16; dbit->Next()) {
      keys.emplace_back(dbit->key().ToString());
      values.emplace_back(dbit->value().ToString());
      cnt++;
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> elapsed = end - start;
    bench_time.add(elapsed.count());
    // Check correctness of results
    for (int i = 0; i < static_cast<int>(keys.size()); i++) {
      if (values[i] != key_value_map[keys[i]]) {
        cerr << "Error: Retrieved value does not match original value." << endl;
        cerr << "Key: \"" << keys[i] << "\"" << endl;
        cerr << "Original Value: \"" << key_value_map[keys[i]] << "\"" << endl;
        cerr << "Retrieved Value: \"" << values[i] << "\"" << endl;
        delete dbit;
        delete db;
        exit(-1);
      }
    }
    delete dbit;

    if ((i + 1) % 1000 == 0) {
      cout << "Range looked up " << (i + 1) << " keys (" << fixed
           << setprecision(2) << (((double)(i + 1) / (double)count) * 100)
           << "%)" << endl;
    }
  }
  add_db_stats(db, bench_time);
  return bench_time;
}
