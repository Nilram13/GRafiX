add_executable(GrafiX_Demo demo.cpp)
target_link_libraries(GrafiX_Demo PRIVATE GrafiX)

add_custom_command(TARGET GrafiX_Demo POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
          "$<TARGET_FILE_DIR:GrafiX_Demo>/assets"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/assets"
          "$<TARGET_FILE_DIR:GrafiX_Demo>/assets"
)
