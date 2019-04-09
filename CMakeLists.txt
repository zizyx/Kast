cmake_minimum_required (VERSION 2.3)

project (Kast)

execute_process(COMMAND "bash" "tools/pre-build.sh"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

message (${PROJECT_SOURCE_DIR})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# Compile googletest
add_subdirectory(googletest)
include_directories(googlemock/include googletest/include)

# set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -Wall -g")


# Kast sources
set (SRCS src/main.cpp)

# Build targets for kast.
add_custom_target(
   kast
   COMMAND make
   WORKING_DIRECTORY $(PROJECT_SOURCE_DIR)/src/
)

# Clean kast only
add_custom_target(
   clean_kast
   COMMAND make clean
   WORKING_DIRECTORY $(PROJECT_SOURCE_DIR)/src/
)


# Create executable with test from sources.
 add_executable(runTest
     tests/SystemTimer_test.cpp
     src/SystemTimer.cpp)

# Needed libraries
 target_link_libraries(runTest
     pthread
     gmock
     gmock_main
     gtest
     gtest_main)
