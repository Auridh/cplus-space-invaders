set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lncurses")

find_package(Curses REQUIRED)
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${CURSES_INCLUDE_DIR}
)
file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
add_executable(cplus-space-invaders ${all_SRCS})
target_link_libraries(cplus-space-invaders ${CURSES_LIBRARIES})