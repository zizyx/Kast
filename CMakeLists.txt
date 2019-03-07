cmake_minimum_required (VERSION 2.3)

project (Kast)

execute_process(COMMAND "bash" "pre-build.sh"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_subdirectory(googletest)
include_directories(googlemock/include googletest/include)

# set (SRCS main.cc)
# set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -Wall -g")

# add_executable (Kast ${SRCS})





# add_subdirectory(Test)
# add_test(alltests Test/run_tests)

option(WITH_TEST "build test" ON)
if(WITH_TEST)
    enable_testing()
endif()





# add_subdirectory(Mock)

# add_executable(html_parser_test
#     http_parser_test.cpp
#     html_parser.h
#     Ihttp_fetch.h
#     http_fetch.h)
# target_link_libraries(html_parser_test
#     pthread)
# target_link_libraries(html_parser_test
#     gmock
#     gmock_main)
# target_link_libraries(html_parser_test
#     gtest
#     gtest_main)
# add_test(html-tests html_parser_test)

# add_test(alltests Mock/run_tests)
