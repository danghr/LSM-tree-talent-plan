# Task 1: Use the LevelDB to implement the following operations

## 1. Sequentially insert 10,000,000 randomly generated keys of 24 bytes associated with a 1,000 bytes value into LevelDB

Use `class Key {private: char data[24]}` to generate a 24-bytes key.

### 1.1 How can you enforce the randomness of the generation?

In `Key`'s constructor, use a `uniform_int_distribution` random generator from `<random>` header.

### 1.2 How can you ensure that the keys are unique?

In `main`, use a `map` to store the key-value.
Regenerate the key if it already exists.
Note that this `map` also stores the key-value pairs, which can be used to check whether the data are correct.

## 2. Point lookup 1,000,000 keys

### 2.1 Ensure the existence of keys

Select keys from the `map` to ensure its existance.

# Task 2

Set single file size to an unlimited number
One SSTable => One Big sorted run
Limit number of files

Either select either no or all compaction result

Get: query all sorted runs, latest first (file name or sequence key?)