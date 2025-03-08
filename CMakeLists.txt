cmake_minimum_required(VERSION 3.10)  # Minimum required CMake version
project(LibraryManagementSystem)      # Project name

set(CMAKE_CXX_STANDARD 17)            # Use C++17
set(CMAKE_CXX_STANDARD_REQUIRED True) # Ensure C++17 is required

add_executable(library_system 
    main.cpp 
)

target_include_directories(library_system PUBLIC include)
