
message("\n===== [src/CMakeLists.txt] start")
message("PROJECT_NAME: " ${PROJECT_NAME})
message("PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
message("PROJECT_BINARY_DIR: " ${PROJECT_BINARY_DIR})
message("CMAKE_CXX_STANDARD: " ${CMAKE_CXX_STANDARD})

aux_source_directory(. SOURCES)
message("SOURCES: ${SOURCES}")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
message("EXECUTABLE_OUTPUT_PATH: ${PROJECT_BINARY_DIR}/bin")

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(spdlog CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    spdlog::spdlog spdlog::spdlog_header_only 
    pthread
)

message("===== [src/CMakeLists.txt] end\n")