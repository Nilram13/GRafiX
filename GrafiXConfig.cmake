get_filename_component(_GRAFIX_ROOT "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" _cfg_lower) # debug / release

set(_GRAFIX_LIB "${_GRAFIX_ROOT}/cmake-build-${_cfg_lower}/GrafiX.lib")

set(_SFML_ROOT   "${_GRAFIX_ROOT}/SFML/${CMAKE_BUILD_TYPE}")
set(_SFML_INC    "${_SFML_ROOT}/include")
set(_SFML_LIBDIR "${_SFML_ROOT}/lib")

add_library(GrafiX::GrafiX INTERFACE IMPORTED)

target_include_directories(GrafiX::GrafiX INTERFACE
  "${_GRAFIX_ROOT}/include"
  "${_SFML_INC}"
)

target_link_libraries(GrafiX::GrafiX INTERFACE
  "${_GRAFIX_LIB}"
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_link_libraries(GrafiX::GrafiX INTERFACE
    "${_SFML_LIBDIR}/sfml-graphics-s-d.lib"
    "${_SFML_LIBDIR}/sfml-window-s-d.lib"
    "${_SFML_LIBDIR}/sfml-system-s-d.lib"
    "${_SFML_LIBDIR}/freetyped.lib"
  )
else()
  target_link_libraries(GrafiX::GrafiX INTERFACE
    "${_SFML_LIBDIR}/sfml-graphics-s.lib"
    "${_SFML_LIBDIR}/sfml-window-s.lib"
    "${_SFML_LIBDIR}/sfml-system-s.lib"
    "${_SFML_LIBDIR}/freetype.lib"
  )
endif()

if (WIN32)
  target_link_libraries(GrafiX::GrafiX INTERFACE
    opengl32
    winmm
    gdi32
  )
endif()

function(grafix_copy_assets target)
  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target}>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${_GRAFIX_ROOT}/assets" "$<TARGET_FILE_DIR:${target}>/assets"
  )
endfunction()
