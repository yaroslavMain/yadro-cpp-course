cmake_minimum_required(VERSION 3.30)
project(yadro)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Google Tests
add_subdirectory(tasks/contrib/googletest)

# Weeks
add_subdirectory(week_1)
add_subdirectory(week_2)
