## Task 1: Use the LevelDB to implement the following operations

### 1. Sequentially insert

Use `generate_n` to generate keys and values for the given size.

#### 1.1 How can you enforce the randomness of the generation?

In `Key`'s constructor, use a `uniform_int_distribution` random generator from `<random>` header.

#### 1.2 How can you ensure that the keys are unique?

In `main`, use a `map` to store the key-value.
Regenerate the key if it already exists.
This `map` also stores the key-value pairs, which can be used to check whether the data are correct.

### 2. Point lookup

#### 2.1 Ensure the existence of keys

Select keys from the `map` to ensure its existance.

## Task 2

Treat an SSTable as a big sorted run for simplified implementation:
- Set single file size to `SIZE_T_MAX`
- Add limit to number of files and total file size in each level

Levels containing overlapping SSTables have properties similar to level 0
- Select all files during compaction and write a single big SSTable to the next layer
  - A natural size ratio of $x$ by merging $x$ files from level $y$ to level $y + 1$
- Get and Seek: Check all SSTables from newest to oldest in a level, where queried key falls in the range of the SSTable
  - Similar to Level 0
  - Future optimization: Two-layer structure (Sorted-run + SSTables)
    - Avoid too large range for a single SSTable
