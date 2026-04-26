# YADRO C++ Course Homeworks

This repository contains homework assignments for the **YADRO C++ Development Course**.

## 📌 Projects

* **Week 1: bitset** - Implementation of a dynamic bitset with automatic resizing, theory operations (union,
  intersection, subset), and support for Move Semantics.
* **Week 2: buffering_writer** - Implementation of a thread-safe, asynchronous file writer using the Pimpl pattern and a
  background worker thread. It utilizes std::condition_variable to efficiently handle buffered I/O operations
* **Week 3:**
    * **Log call** - A compile-time utility that logs function names to std::cout if they match a predefined interesting list.
    * **User-literal _rpn** - A custom literal that parses and evaluates Reverse Polish Notation expressions into integers at compile time.
    * **User-literal _ti** - A recursive-descent parser that converts a string literal (using < | > syntax) into a dynamic binary tree of integers.
    * **User-literal _vi** - An easy-to-use literal that parses a delimited string (e.g., "1,2;3.4") and returns a std::vector<int>
* **Week 4**: A custom implementation of a std::tuple-like container using variadic templates and recursive inheritance.

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
