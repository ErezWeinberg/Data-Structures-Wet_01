# Data Structures - Wet Exercise #1

## Course Information
- **Course**: 234218 Data Structures 1
- **Semester**: 2025A (Winter)
- **Exercise**: Wet Exercise #1

## Project Overview

This project implements a **Plains Management System** that manages herds of horses. The system tracks horses, their herds, and the leadership relationships between horses within herds.

### Key Features
- Add and remove herds
- Add horses with specific speeds
- Horses can join and leave herds
- Horses can follow other horses within the same herd
- Query horse speeds and leadership relationships
- Check if all horses in a herd can run together

## Data Structures

The implementation uses **AVL Trees** (self-balancing binary search trees) for efficient operations:

1. **Global Horse Tree**: Stores all horses in the system, indexed by horse ID
2. **Global Herd Tree**: Stores all herds in the system, indexed by herd ID
3. **Herd-specific Horse Trees**: Each herd maintains its own AVL tree of horses that belong to it

### Node Structures

#### HorseNode
- `id`: Unique identifier for the horse
- `speed`: Speed of the horse
- `left`, `right`: Pointers to left and right children in AVL tree
- `height`: Height of the node in AVL tree (for balancing)
- `herd`: Pointer to the herd this horse belongs to
- `following`: Pointer to the horse this horse is following
- `followers`: Linked list of horses following this horse

#### HerdNode
- `id`: Unique identifier for the herd
- `horses`: Root of AVL tree containing horses in this herd
- `left`, `right`: Pointers to left and right children in AVL tree
- `height`: Height of the node in AVL tree (for balancing)

## API Functions

### `add_herd(int herdId)`
Adds a new herd to the system.
- **Input**: `herdId` - Unique identifier for the herd (must be > 0)
- **Returns**: 
  - `SUCCESS` - Herd added successfully
  - `INVALID_INPUT` - herdId ≤ 0
  - `FAILURE` - Herd already exists
  - `ALLOCATION_ERROR` - Memory allocation failed

### `remove_herd(int herdId)`
Removes an empty herd from the system.
- **Input**: `herdId` - Identifier of the herd to remove
- **Returns**: 
  - `SUCCESS` - Herd removed successfully
  - `INVALID_INPUT` - herdId ≤ 0
  - `FAILURE` - Herd doesn't exist or is not empty
  - `ALLOCATION_ERROR` - Operation failed

### `add_horse(int horseId, int speed)`
Adds a new horse to the system.
- **Input**: 
  - `horseId` - Unique identifier for the horse (must be > 0)
  - `speed` - Speed of the horse (must be > 0)
- **Returns**: 
  - `SUCCESS` - Horse added successfully
  - `INVALID_INPUT` - horseId ≤ 0 or speed ≤ 0
  - `FAILURE` - Horse already exists
  - `ALLOCATION_ERROR` - Memory allocation failed

### `join_herd(int horseId, int herdId)`
Makes a horse join a herd.
- **Input**: 
  - `horseId` - Identifier of the horse
  - `herdId` - Identifier of the herd
- **Returns**: 
  - `SUCCESS` - Horse joined herd successfully
  - `INVALID_INPUT` - horseId ≤ 0 or herdId ≤ 0
  - `FAILURE` - Horse or herd doesn't exist, or horse already in a herd
  - `ALLOCATION_ERROR` - Memory allocation failed

### `leave_herd(int horseId)`
Makes a horse leave its current herd.
- **Input**: `horseId` - Identifier of the horse
- **Returns**: 
  - `SUCCESS` - Horse left herd successfully
  - `INVALID_INPUT` - horseId ≤ 0
  - `FAILURE` - Horse doesn't exist or is not in a herd
  - `ALLOCATION_ERROR` - Operation failed

### `follow(int horseId, int horseToFollowId)`
Makes one horse follow another horse in the same herd.
- **Input**: 
  - `horseId` - Identifier of the horse that will follow
  - `horseToFollowId` - Identifier of the horse to be followed
- **Returns**: 
  - `SUCCESS` - Follow relationship established
  - `INVALID_INPUT` - Invalid IDs or same horse
  - `FAILURE` - Horses don't exist, not in same herd, or not in any herd
  - `ALLOCATION_ERROR` - Memory allocation failed

### `get_speed(int horseId)`
Gets the speed of a horse.
- **Input**: `horseId` - Identifier of the horse
- **Returns**: `output_t<int>` with:
  - `SUCCESS` and speed value - If horse exists
  - `INVALID_INPUT` - horseId ≤ 0
  - `FAILURE` - Horse doesn't exist

### `leads(int horseId, int otherHorseId)`
Checks if one horse leads another (directly or indirectly through the following chain).
- **Input**: 
  - `horseId` - Identifier of the potential leader
  - `otherHorseId` - Identifier of the potential follower
- **Returns**: `output_t<bool>` with:
  - `SUCCESS` and true/false - If both horses exist
  - `INVALID_INPUT` - Invalid IDs or same horse
  - `FAILURE` - One or both horses don't exist

### `can_run_together(int herdId)`
Checks if all horses in a herd can run together. This is true if there exists a horse that leads all other horses in the herd.
- **Input**: `herdId` - Identifier of the herd
- **Returns**: `output_t<bool>` with:
  - `SUCCESS` and true/false - If herd exists and has horses
  - `INVALID_INPUT` - herdId ≤ 0
  - `FAILURE` - Herd doesn't exist or is empty

## Compilation

### Using CMake (Recommended)
```bash
mkdir build
cd build
cmake ..
make
```

### Manual Compilation
```bash
g++ -std=c++14 -Wall -o plains main25a1.cpp plains25a1.cpp
```

## Running the Program

The program reads commands from standard input:

```bash
./plains < input_file.txt
```

### Input Format
Each line contains a command and its arguments:
```
add_herd <herdId>
remove_herd <herdId>
add_horse <horseId> <speed>
join_herd <horseId> <herdId>
follow <horseId> <horseToFollowId>
leave_herd <horseId>
get_speed <horseId>
leads <horseId> <otherHorseId>
can_run_together <herdId>
```

### Output Format
Each command produces output in the format:
```
<command>: <status>[, <result>]
```

Where status is one of: SUCCESS, INVALID_INPUT, FAILURE, ALLOCATION_ERROR

## Testing

Test files are located in the `tests/` directory:
- `test0.in`, `test0.out` - Basic functionality tests
- `test10.in`, `test10.out` - Additional test cases
- `test20.in`, `test20.out` - More complex scenarios
- `test30.in`, `test30.out` - Edge cases
- `test40.in`, `test40.out` - Performance tests

### Running Tests
```bash
./plains < tests/test0.in > output.txt
diff output.txt tests/test0.out
```

Or use the Python test runner:
```bash
python3 run_tests.py
```

## Project Structure

```
.
├── CMakeLists.txt          # CMake build configuration
├── README.md               # This file
├── plains25a1.h           # Header file with class declarations
├── plains25a1.cpp         # Implementation of Plains class
├── main25a1.cpp           # Main program with I/O handling
├── wet1util.h             # Utility definitions
├── tests/                 # Test input/output files
│   ├── test0.in
│   ├── test0.out
│   └── ...
└── run_tests.py           # Test runner script
```

## Implementation Details

### AVL Tree Operations
- **Insertion**: O(log n) - with automatic balancing
- **Deletion**: O(log n) - with automatic balancing
- **Search**: O(log n)
- **Rotations**: Left, Right, Left-Right, Right-Left

### Leadership Tracking
- Uses a linked list to track followers of each horse
- Uses pointer to track which horse a horse is following
- `doesLead()` function performs path traversal to check leadership chains
- Prevents cycles using visited array

### Memory Management
- Proper cleanup in destructor
- Prevents double deletion of shared data structures
- Exception handling for allocation errors

## Complexity Analysis

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| add_herd | O(log H) | O(1) |
| remove_herd | O(log H) | O(1) |
| add_horse | O(log N) | O(1) |
| join_herd | O(log N + log H + log M) | O(1) |
| leave_herd | O(log M + F) | O(1) |
| follow | O(log N + F) | O(1) |
| get_speed | O(log N) | O(1) |
| leads | O(N) worst case | O(N) |
| can_run_together | O(M² × N) worst case | O(N) |

Where:
- H = number of herds
- N = total number of horses
- M = number of horses in a specific herd
- F = number of followers of a horse

## Notes

- All IDs must be positive integers (> 0)
- A horse can only belong to one herd at a time
- A horse can only follow one horse at a time
- A horse can have multiple followers
- Leadership is transitive (if A leads B and B leads C, then A leads C)
- The system prevents cycles in following relationships within the same herd

## Author

This is an academic exercise for Data Structures course 234218.

## License

This code is for educational purposes only.
