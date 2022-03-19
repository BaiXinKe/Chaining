cmake_minimum_required(VERSION 3.0.0)
project(Chaining VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED 11)

set(SRC_ROOT_DIR src)

set(SOURCE ${SRC_ROOT_DIR}/main.cpp ${SRC_ROOT_DIR}/auxiliary/ReadSetting.cpp)

add_executable(Chaining  ${SOURCE})

target_compile_options(Chaining PRIVATE "-fcoroutines" )

# ---------------- link local libary ------------------------ #




# ---------------- link package ----------------------------- #

find_package(Threads REQUIRED)
if(Threads_FOUND)
target_link_libraries(Chaining Threads::Threads)
else()
message (FATAL_ERROR "Threads Not Found")
endif()


# ------------------ unit testing --------------------------- #
enable_testing()

set(TESTING_SOURCE_DIR testing)

add_executable(utest ${TESTING_SOURCE_DIR}/utest.cpp)

find_package(GTest REQUIRED)
if(GTest_FOUND)
    target_link_libraries(utest GTest::GTest GTest::Main)
else()
    message (FATAL_ERROR "GTest Not Found")
endif()

add_executable(convertTest ${TESTING_SOURCE_DIR}/convertTest.cpp ${SRC_ROOT_DIR}/auxiliary/ReadSetting.cpp)
target_link_libraries(convertTest GTest::GTest GTest::Main)

add_test(test_all utest convertTest)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
