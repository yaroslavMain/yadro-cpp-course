# YADRO C++ Course Homeworks

This repository contains homework assignments for the **YADRO C++ Development Course**.

## 📌 Projects

* **Lesson 1: bitset** - Implementation of a dynamic bitset with automatic resizing, theory operations (union, intersection, subset), and support for Move Semantics.

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
