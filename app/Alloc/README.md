# Hostel Room Allocation System - Algorithm Documentation

## Overview
This system provides three different algorithms for allocating hostel rooms and grouping them based on student preferences and compatibility factors.

## Algorithms Implemented

### 1. Basic Improved Algorithm (`improved_allocation.cpp`)
**Purpose**: Enhanced version of the original algorithm with better preference handling.

**Features**:
- Prioritizes mutual preferences for room allocation
- Falls back to one-way preferences
- Random pairing for remaining students
- Basic group formation based on remaining preferences

**Process**:
1. Load student data and pod preferences
2. First pass: Mutual preference pairing
3. Second pass: One-way preference pairing
4. Third pass: Random pairing for leftover students
5. Group formation based on cross-room preferences

**Output**: `data/rooms.csv`

### 2. Weighted Graph Algorithm (`weighted_allocation.cpp`)
**Purpose**: Treats room allocation as a weighted graph problem considering spatial proximity.

**Features**:
- Simulates physical hostel layout with coordinates
- Calculates distance between rooms
- Combines preference scores with compatibility metrics
- Optimizes for both preference satisfaction and physical proximity

**Key Components**:
- **Room Coordinates**: Simulates a 3-floor hostel layout
- **Distance Calculation**: Euclidean distance between rooms
- **Preference Scoring**: Weighted by preference order (1st choice = higher weight)
- **Compatibility Scoring**: Based on gender, branch, study year, floor, lift, mess, and window preferences

**Scoring System**:
- Mutual preferences: +100 bonus
- Preference rank: (6 - rank) × 20 points
- Same gender: +10 points
- Same branch: +5 points
- Same study year: +5 points
- Same floor preference: +15 points
- Distance penalty: -0.5 × distance

**Output**: `data/weighted_rooms.csv`, `data/group_details.csv`

### 3. Comprehensive Algorithm (`comprehensive_allocation.cpp`)
**Purpose**: Most advanced algorithm considering all factors with detailed compatibility analysis.

**Features**:
- Comprehensive compatibility scoring system
- Mandatory gender matching
- Detailed preference weight calculations
- Advanced group formation with satisfaction metrics
- Detailed analysis and reporting

**Compatibility Factors**:
- **Gender**: Mandatory matching (invalid if different)
- **Branch**: Same branch (+20), related engineering fields (+10)
- **Study Year**: Same year (+15), adjacent years (+8)
- **Floor**: Same preference (+25), no preference (+10)
- **Lift Proximity**: Score based on preference similarity
- **Mess Proximity**: Score based on preference similarity
- **Window**: Same preference (+10), no preference (+5)
- **Degree**: Same degree (+10)

**Preference Weight System**:
- Proximity values: very near (5), near (4), no preference (3), far (2), very far (1)
- Floor values: Ground (4), First (3), Basement (2), No Preference (1)
- Preference order: Higher positions get more weight

**Advanced Features**:
- Priority queue for optimal pairing
- Cross-room preference analysis for groups
- Satisfaction metrics calculation
- Detailed reporting with scores

**Output**: `data/comprehensive_rooms.csv`, `data/comprehensive_groups.csv`

## File Structure

```
app/Alloc/
├── improved_allocation.cpp     # Basic improved algorithm
├── weighted_allocation.cpp     # Weighted graph algorithm
├── comprehensive_allocation.cpp # Comprehensive algorithm
├── Makefile                   # Build configuration
├── run_all_algorithms.sh      # Script to run all algorithms
└── data/                      # Input and output data
    ├── choices.csv           # Student preferences
    ├── pod_*_table.csv       # Pod preferences (1-6)
    ├── rooms.csv             # Basic algorithm output
    ├── weighted_rooms.csv    # Weighted algorithm output
    ├── comprehensive_rooms.csv # Comprehensive algorithm output
    ├── group_details.csv     # Group analysis
    └── comprehensive_groups.csv # Detailed group analysis
```

## Usage

### Compilation
```bash
make all
```

### Running Individual Algorithms
```bash
./improved_allocation
./weighted_allocation
./comprehensive_allocation
```

### Running All Algorithms
```bash
make run
```

### Viewing Results
```bash
make show-results
```

## Output File Formats

### Basic Algorithm Output (`rooms.csv`)
```csv
room_no,user1_id,user2_id,user1_name,user2_name,group_no
```

### Weighted Algorithm Output (`weighted_rooms.csv`)
```csv
room_no,user1_id,user2_id,user1_name,user2_name,group_no,x_coord,y_coord
```

### Comprehensive Algorithm Output (`comprehensive_rooms.csv`)
```csv
room_no,user1_id,user2_id,user1_name,user2_name,group_no,compatibility_score,assigned_floor,lift_proximity,mess_proximity,has_outside_window
```

### Group Analysis (`comprehensive_groups.csv`)
```csv
group_no,room1,room2,overall_satisfaction,compatibility_score,preference_match
```

## Algorithm Performance Comparison

Based on test results:
- **Basic**: Fast execution, basic preference matching
- **Weighted**: Moderate complexity, considers spatial layout
- **Comprehensive**: Highest complexity, maximum satisfaction optimization

**Satisfaction Scores** (from test run):
- Average room satisfaction: 87.9
- Average group satisfaction: 423.6

## Key Improvements Over Original

1. **Better Data Handling**: Proper CSV parsing and error handling
2. **Preference Prioritization**: Mutual preferences get highest priority
3. **Compatibility Analysis**: Multiple factors considered beyond just preferences
4. **Spatial Awareness**: Physical room layout consideration
5. **Detailed Reporting**: Comprehensive analysis and metrics
6. **Flexible Architecture**: Easy to extend with new algorithms

## Future Enhancements

- Add more complex constraint satisfaction
- Implement machine learning for pattern recognition
- Add real-time preference updating
- Include more detailed hostel layout modeling
- Add visualization tools for results
