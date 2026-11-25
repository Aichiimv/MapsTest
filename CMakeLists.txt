cmake_minimum_required(VERSION 3.16)
project(pi_maps LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find system-installed libraries
find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(pi_maps main.cpp)

target_link_libraries(pi_maps PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

# Enable helpful compiler warnings
target_compile_options(pi_maps PRIVATE -Wall -Wextra)