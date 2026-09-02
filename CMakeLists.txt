cmake_minimum_required(VERSION 3.20)
project(GrafiX VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

option(GRAFIX_BUILD_DEMO "Build GrafiX demo executable" ON)

# Where you want SFML to live permanently inside this repo
set(GRAFIX_SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML")

# Determine config name (Debug/Release)
if (CMAKE_CONFIGURATION_TYPES)
  set(_cfg "$<CONFIG>")
else()
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
  endif()
  set(_cfg "${CMAKE_BUILD_TYPE}")
endif()

# ------------------------------------------------------------
# Assets live in REPO ROOT: /assets
# ------------------------------------------------------------
set(GRAFIX_FONT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/assets/front.ttf")
if (NOT EXISTS "${GRAFIX_FONT_FILE}")
  message(FATAL_ERROR "GrafiX: Missing font file: ${GRAFIX_FONT_FILE}")
endif()

# ------------------------------------------------------------
# SFML: ALWAYS fetch 3.0.2 from official repo (ignore SFML_DIR)
# ------------------------------------------------------------
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)

# Recommended: static build -> easiest reuse (no runtime DLL copying)
option(GRAFIX_SFML_STATIC "Build SFML static (recommended)" ON)
if (GRAFIX_SFML_STATIC)
  set(SFML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
else()
  set(SFML_BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
endif()

FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
)
FetchContent_MakeAvailable(SFML)

# ------------------------------------------------------------
# GrafiX library
# ------------------------------------------------------------
add_library(GrafiX STATIC GrafiX.cpp)
add_library(GrafiX::GrafiX ALIAS GrafiX)

target_include_directories(GrafiX PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(GrafiX PUBLIC
        SFML::Graphics
        SFML::Window
        SFML::System
)

if (WIN32)
  target_compile_definitions(GrafiX PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

# ------------------------------------------------------------
# KORREKTUR: Kopieren der Header & echten Targets
# ------------------------------------------------------------
# Nutze die echten Target-Namen (sfml-system statt SFML::System)
# Hinweis: Wenn SHARED_LIBS OFF ist, kopiert das die .lib-Dateien.
add_custom_command(TARGET GrafiX POST_BUILD
        # include/
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${GRAFIX_SFML_ROOT}/${_cfg}/include"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${sfml_SOURCE_DIR}/include"
        "${GRAFIX_SFML_ROOT}/${_cfg}/include"

        # lib/
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${GRAFIX_SFML_ROOT}/${_cfg}/lib"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:sfml-system>"
        "$<TARGET_FILE:sfml-window>"
        "$<TARGET_FILE:sfml-graphics>"
        "${GRAFIX_SFML_ROOT}/${_cfg}/lib"

        COMMENT "Staging SFML into ${GRAFIX_SFML_ROOT}/${_cfg}"
)

# ------------------------------------------------------------
# Demo in /examples (optional)
# ------------------------------------------------------------
if (GRAFIX_BUILD_DEMO)
  add_subdirectory(examples)
endif()
