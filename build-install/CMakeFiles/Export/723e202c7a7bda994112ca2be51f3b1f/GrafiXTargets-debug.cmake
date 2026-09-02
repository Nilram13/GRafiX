#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GrafiX::GrafiX" for configuration "Debug"
set_property(TARGET GrafiX::GrafiX APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(GrafiX::GrafiX PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/GrafiX.lib"
  )

list(APPEND _cmake_import_check_targets GrafiX::GrafiX )
list(APPEND _cmake_import_check_files_for_GrafiX::GrafiX "${_IMPORT_PREFIX}/lib/GrafiX.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
