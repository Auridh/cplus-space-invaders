enable_testing()

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lncurses -lboost_unit_test_framework")

include_directories (
        ${PROJECT_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
)

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/controller_console.cpp"
        "${PROJECT_SOURCE_DIR}/src/model_simulator_game.cpp"
        "${PROJECT_SOURCE_DIR}/src/observer.cpp"
        "${PROJECT_SOURCE_DIR}/src/view_console.cpp"
)
add_executable(test test_call.cpp ${all_SRCS})

target_link_libraries(test
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_test(MyTest test)