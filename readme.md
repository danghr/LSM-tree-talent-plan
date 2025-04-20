# A talent plan preparing you for the academic research of LSM-tree
LSM-tree (Log-Structured Merge-tree) is a data structure that is widely used in databases and storage systems for efficient write operations and read performance. It is particularly well-suited for applications with high write throughput and large amounts of data. The LSM-tree is designed to handle large volumes of data by organizing it into multiple levels, allowing for efficient merging and compaction processes.

**This plan is designed to help you prepare for academic research in LSM-tree. It covers the essential topics, concepts, and skills needed to understand and contribute to the field of LSM-tree research.**

> This project does not provide details of the basis of LSM-tree. If you have not learned the basis of LSM-tree, please Google it (or use ChatGPT) to learn the basis of LSM-tree.

This project contains a total of three tasks. Each task is designed to help you learn and understand the key concepts and techniques related to LSM-tree. The tasks are organized in a way that allows you to gradually build your knowledge and skills in this area.
The tasks are as follows:

- Task 1: Use the LevelDB to implement the following operations:
  - Insert new keys
  - Delete keys
  - Update keys
  - Read the value of a specific key
  - Range lookup some keys (e.g., get all keys in a specific range)
- Task 2: Use LevelDB to implement Tiering Compaction
- Task 3: Use LevelDB to implement Monkey Filter
- (Optional) Task 4: Use LevelDB to implement your own design and justify why it is better.

<span style="color: #e02d2d">Regarding the questions listed in each task, we encourage you to write it down in a report. This will help you better clarify your thoughts and present your findings, which is more important than writing code during your PhD study.</span>

## Task 1: Use the LevelDB to implement basic operations
As a key-value store, LSM-tree supports mainly five operations, insert, delete, update, point lookup, and range lookup. In this task, you are going to use LevelDB to implement the following functions:

1. Sequentially insert 10,000,000 randomly generated keys of 24 bytes associated with a 1,000 bytes value into LevelDB.
1.1. How can you enforce the randomness of the generation?
1.2. How can you ensure that the keys are unique?
2. Point lookup 1,000,000 keys
2.1. You should check whether the key exists
3. Sequentially delete 1,000,000 keys
4. Sequentially update 1,000,000 keys
5. Range lookup 1,000,000 keys
5.1. Randomly generate 1,000,000 start keys and then sequentially scan the next 16 keys for each start key.
6. Report the performance of each operations you tested above.
6.1. You should report the latency of each operation.

After completing this task, you should be able to understand the basic operations of LSM-tree and how to use LevelDB to implement them. During the implementation and testing, do you have any insights or questions about the performance of LSM-tree?

## Task 2: Use LevelDB to implement Tiering Compaction
In this task, you are going to implement The Tiering Compaction in LevelDB. Tiering Compaction organizes data into multiple sorted runs at a level, compacts them into a single sorted run and places the result into the next level if the size of a level exceeds a certain threshold. Typically, we set the size ratio of two adjacent levels to be 10 (e.g., $T=10$) and the maximal allowing number of sorted runs at a level to be 10. Let's define the maximal size of the first level (L0) to be $\text{buffer\_size} \times 10$, and each level $L_i$ ($i \geq 1$) to be $10^
{i+1} \times \text{buffer\_size}$. Please implement the following functions:

1. Set the buffer size to 2MB.
2. Set the size ratio of two adjacent levels to be 10.
3. Set the maximal allowing number of sorted runs at a level to be 10.
4. Sequentially insert 10,000,000 randomly generated unqiue keys of 24 bytes associated with a 1,000 bytes value into LevelDB.
5. Justify the final state of the LSM-tree after the insertion.
5.1. How many levels are there in the LSM-tree?
5.2. How many sorted runs are there in each level?
5.3. What is the write amplification?
6. Randomly point lookup 1,000,000 keys
6.1. You should check whether the key exists
7. Randomly range lookup 1,000,000 keys
7.1. Randomly generate 1,000,000 start keys and then sequentially scan the next 16 keys for each start key.
8. Report the performance of each operations you tested above.

**Hint: besides implementing the Tiering Compaction, you should garauntee the correctness of read.**

## Task 3: Use LevelDB to implement Monkey Filter
[Monkey Filter](https://dl.acm.org/doi/10.1145/3035918.3064054) is a new filter enabling the LSM-tree to set different bits-per-key for different levels and consequently enhance the overall performance of the LSM-tree. In this task, you are going to implement the Monkey Filter in LevelDB. Please implement the following functions:

1. Implement a new filter called `MonkeyFilter` implementing the original Filter interface.
2. You can replace the default filter in LevelDB by modifying the `Options`.
3. You are able to set different bits-per-key for different levels.
4. Implement the optimal algorithm in the paper to derive the optimal settings of bits-per-key for different levels.
5. Sequentially insert 10,000,000 randomly generated unqiue keys of 24 bytes associated with a 1,000 bytes value into LevelDB and then do empty point lookups (i.e., the key does not exists):
5.1. Report the performance of the Monkey Filter and the default filter in LevelDB (i.e., the latency of point lookups, the false positive rate, the memory usage for the filters).

## (Optional) Task 4: Use LevelDB to implement your own design and justify why it is better
After completing the first three tasks, I believe you already have a good understanding of the LSM-tree and its design. Based on the findings above and your own understanding, do you have any good ideas to further improve the performance of LSM-tree?

## Recommending Papers
[Monkey: Optimal Navigable Key-Value Store](https://dl.acm.org/doi/pdf/10.1145/3035918.3064054)
[Dostoevsky: Better Space-Time Trade-Offs for LSM-Tree Based Key-Value Stores via Adaptive Removal of Superfluous Merging](https://scholar.harvard.edu/files/stratos/files/dostoevskykv.pdf)
[The Log-Structured Merge Bush & the Wacky Continuum](https://dl.acm.org/doi/pdf/10.1145/3299869.3319903)
[Structural Designs Meet Optimality: Exploring Optimized LSM-tree Structures in A Colossal Configuration Space](https://dl.acm.org/doi/pdf/10.1145/3654978)
[Spooky: Granulating LSM-Tree Compactions Correctly](https://vldb.org/pvldb/vol15/p3071-dayan.pdf)