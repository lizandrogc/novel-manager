# Novel Manager

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)

> **Status:** Work in Progress

A terminal-based novel management system written in **C**, developed as the final project for the **Algorithms and Data Structures I** course at the **Federal University of Goiás (UFG)**.

## Overview

Novel Manager is a command-line application designed to organize web novels and light novels, track reading progress, and manage reading status.

The project focuses on applying fundamental data structures and algorithms while exploring modular programming, dynamic memory management, and software organization in C.

## Core Concepts

- **Data Structures** (Linked Lists, Stacks, Queues)
- **Pointers & Dynamic Memory**
- **Structs & Data Modeling**
- **Modular Programming** (`.c` and `.h` files)
- **Testing & Input Validation**

## Development Progress

- [x] **Current Reads** — Doubly Linked List
- [x] **Completed Reads** — Singly Linked List
- [x] **Next Reads** — Queue
- [x] **Favorites** — Static List
- [ ] **Undo History** — Stack
- [ ] **File Persistence** — Text File
- [ ] **CLI Integration** — Terminal Interface
- [ ] **Final Review** — Testing and Documentation

Development is organized into independent stages, with each data structure implemented and tested before integration.

## Building & Testing

The project is currently under development. Individual modules can be compiled and tested using GCC.

### Current Reads

```bash
gcc -std=c11 -Wall -Wextra -Iinclude tests/test_current_reads.c src/current_reads.c -o tests/current_reads.exe

./tests/current_reads.exe
```

### Completed Reads

```bash
gcc -std=c11 -Wall -Wextra -Iinclude tests/test_completed_reads.c src/completed_reads.c -o tests/completed_reads.exe

./tests/completed_reads.exe
```

### Next Reads

```bash
gcc -std=c11 -Wall -Wextra -Iinclude tests/test_next_reads.c src/next_reads.c -o tests/next_reads.exe

./tests/next_reads.exe
```

### Favorites

```bash
gcc -std=c11 -Wall -Wextra -Iinclude tests/test_favorites_reads.c src/favorites_reads.c -o tests/favorites_reads.exe

./tests/favorites_reads.exe
```

All four modules have been tested using assertions to verify their operations, input validation, edge cases, and data structure integrity.