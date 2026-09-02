cmake_minimum_required(VERSION 3.20)
project(GFX_consumer_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(GFX_consumer_example main.cpp)

find_package(GrafiX REQUIRED PATHS "C:/Users/{YOUR_USERNAME}/CLionProjects/GrafiX")
target_link_libraries(GFX_consumer_example PRIVATE GrafiX::GrafiX)
grafix_copy_assets(GFX_consumer_example)