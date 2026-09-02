cmake_minimum_required(VERSION 3.20)
project(GFX_consumer_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. GRafiX als Unterprojekt einbinden (Achte auf das große R bei GRafiX)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../GRafiX" GRafiX_build)

# 2. Executable erstellen
add_executable(GFX_consumer_example main.cpp)

# 3. Verlinken
target_link_libraries(GFX_consumer_example PRIVATE GrafiX::GrafiX)

# 4. Lokalen Assets-Ordner anlegen, falls nicht vorhanden (verhindert Kopierfehler)
file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets")

# 5. Assets (sowohl aus GRafiX als auch lokal) automatisch in den Build-Ordner kopieren
add_custom_command(
        TARGET GFX_consumer_example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:GFX_consumer_example>/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../GRafiX/assets"
        "$<TARGET_FILE_DIR:GFX_consumer_example>/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:GFX_consumer_example>/assets"
)