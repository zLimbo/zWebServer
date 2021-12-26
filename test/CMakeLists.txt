message("\n===== [test/CMakeLists.txt] start")
message("PROJECT_NAME: " ${PROJECT_NAME})
message("PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
message("PROJECT_BINARY_DIR: " ${PROJECT_BINARY_DIR})
message("CMAKE_CXX_STANDARD: " ${CMAKE_CXX_STANDARD})

# file(GLOB_RECURSE ALL_TEST_SRC *.cpp *.cc)
aux_source_directory(. ALL_TEST_SRC)
message("ALL_TEST_SRC: " ${ALL_TEST_SRC})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
message("EXECUTABLE_OUTPUT_PATH: ${PROJECT_BINARY_DIR}/bin")

find_package(spdlog CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

foreach (TEST_FILE ${ALL_TEST_SRC})
    get_filename_component(TEST_FILE_NAME ${TEST_FILE} NAME_WE)
    message("Build ${TEST_FILE_NAME} according to ${TEST_FILE}")
    add_executable(${TEST_FILE_NAME} ${TEST_FILE})
    target_include_directories(${TEST_FILE_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

    target_link_libraries(${TEST_FILE_NAME} PRIVATE 
        pthread
        mysqlclient
        spdlog::spdlog spdlog::spdlog_header_only 
        GTest::gmock GTest::gmock_main GTest::gtest_main
    )
    
endforeach(TEST_FILE)

message("===== [test/CMakeLists.txt] end\n")