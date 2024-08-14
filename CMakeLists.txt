include_directories("${PROJECT_SOURCE_DIR}/include/")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/")
add_executable(${EXECUTABLE_NAME} src/main.cpp ${SOURCE})