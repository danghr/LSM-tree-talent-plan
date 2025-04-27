#ifndef TASK1_OPERATION_H_
#define TASK1_OPERATION_H_

#include <leveldb/db.h>
#include <leveldb/iterator.h>
#include <leveldb/status.h>
#include <map>
#include <string>

#include "bench_time.h"

BenchTime fillrandom(leveldb::DB* db, leveldb::WriteOptions& write_options,
                     std::map<std::string, std::string>& key_value_map,
                     int count);

BenchTime readrandom(leveldb::DB* db, leveldb::ReadOptions& read_options,
                     std::map<std::string, std::string>& key_value_map,
                     int count);

BenchTime delrandom(leveldb::DB* db, leveldb::WriteOptions& write_options,
                    std::map<std::string, std::string>& key_value_map,
                    int count);

BenchTime updaterandom(leveldb::DB* db, leveldb::WriteOptions& write_options,
                       std::map<std::string, std::string>& key_value_map,
                       int count);

BenchTime rangequery(leveldb::DB* db, leveldb::ReadOptions& read_options,
                     std::map<std::string, std::string>& key_value_map,
                     int count);

#endif