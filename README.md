# C++ Data Structures

Selected UMBC CMSC 341 coursework implementations, grouped in one repository to keep the portfolio focused.

| Folder | Subject |
|---|---|
| `solitaire` | Solitaire board implementation |
| `snakes-and-ladders` | Game representation and operations |
| `robot-swarm-trees` | BST, AVL, and splay tree interfaces for robot records |
| `priority-order-queue` | Skew/leftist heap interfaces and order priorities |
| `file-hash-table` | File records, open addressing, and collision policies |

Each folder includes its original implementation, header, demonstration driver, and test program. Course scaffolding/interface comments are retained; this repository does not claim that provided course interfaces were independently designed.

## Requirements

A C++17 compiler, such as g++. Build each project separately; see its README.

## Verification during preparation

- `solitaire`: Compiled; test executable ran (exit -11).
- `snakes-and-ladders`: Compiled; test executable ran (exit -11).
- `robot-swarm-trees`: Compiled; test executable ran (exit 0).
- `priority-order-queue`: Compiled; test executable ran (exit 0).
- `file-hash-table`: Compiled; test executable ran (exit 0).

A compilation or zero exit code alone is not proof that every data structure invariant is correct. Inspect test output when evaluating the implementations.
