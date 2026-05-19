# YADRO C++ Course Homeworks

This repository contains homework assignments for the **YADRO C++ Development Course**.

## 📌 Projects

* **Week 1: bitset** - Implementation of a dynamic bitset with automatic resizing, theory operations (union,
  intersection, subset), and support for Move Semantics.
* **Week 2: buffering_writer** - Implementation of a thread-safe, asynchronous file writer using the Pimpl pattern and a
  background worker thread. It utilizes std::condition_variable to efficiently handle buffered I/O operations
* **Week 3:**
    * **Log call** - A compile-time utility that logs function names to std::cout if they match a predefined interesting
      list.
    * **User-literal _rpn** - A custom literal that parses and evaluates Reverse Polish Notation expressions into
      integers at compile time.
    * **User-literal _ti** - A recursive-descent parser that converts a string literal (using < | > syntax) into a
      dynamic binary tree of integers.
    * **User-literal _vi** - An easy-to-use literal that parses a delimited string (e.g., "1,2;3.4") and returns a std::
      vector<int>
* **Week 4**: **tuple** - A custom implementation of a std::tuple-like container using variadic templates and recursive
  inheritance.
* **Week 5 and 6**:
    * **Meta computer** - A compile-time utility that evaluates expressions during compilation using template
      metaprogramming.
    * **Smart sort** - A practical example of metaprogramming that selects the most efficient sorting algorithm at
      compile
      time based on the object type.
* **Week 7**:
    * **Action Store** - A robust, lightweight implementation of a move-only callable wrapper designed to decoupling
      event systems and pipeline architectures without classical OOP inheritance overhead.
    * **std::function Replica** - A modern C++ type-erased wrapper for callable entities.
    * **Serialization Wrapper** - A generic type-erased wrapper that provides a unified `serialize` interface for any
      custom user types.
* **Week 8**:
    * **Basic Example** - C++ template-based thread pool utilizing a synchronized queue and condition variables for
      concurrent task execution.
    * **Shared Data System** - A thread-safe data container powered by a custom write-preferred reader-writer mutex to
      prioritize updates over lookups.
* **Week 9**:
    * **Bounded Queue** - A thread-safe, capacity-constrained queue featuring both blocking and non-blocking operations
      managed by two distinct condition variables.
    * **Consumer-Producer** - A synchronized workflow pipeline built around a graceful shutdown mechanism that safely
      unblocks waiting threads when production ends.
    * **Periodic Timer** - An asynchronous execution container that utilizes a dedicated background thread and
      conditional timed waits to trigger repetitive callbacks.
    * **Read-Write Lock** - A custom shared-exclusive synchronization primitive with custom RAII guards designed to
      prevent reader starvation by prioritizing pending writer threads.

## ⚠️ Important: Git Submodules

This project uses **Git Submodules** for external dependencies (testing framework GTest).

To ensure the project builds correctly, please clone the repository with the `--recursive` flag:

```bash
git clone --recursive https://github.com/yaroslavMain/yadro-cpp-course.git
```

If you have already cloned the repository without this flag, run the following command to fetch the submodules:

````bash
git submodule update --init --recursive
````
