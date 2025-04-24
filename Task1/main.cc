#include <iostream>
#include <leveldb/db.h>
#include <leveldb/status.h>

using namespace std;

int main() {
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "db", &db);
    if (!status.ok()) {
        cerr << "Unable to open/create test database." << endl;
        cerr << status.ToString() << endl;
        return 1;
    }
    cout << "Database opened successfully." << endl;
    return 0;
}