include(GenerateExportHeader)

find_package(Qt5Core)

file(GLOB_RECURSE lib_includes "include/*.h")
file(GLOB_RECURSE lib_sources "src/*.cpp")

add_library(logic SHARED ${lib_includes} ${lib_sources})

generate_export_header(logic EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/exports/logic_export.h")

target_include_directories(logic PUBLIC "include" "src" "${CMAKE_BINARY_DIR}/exports")
target_link_libraries(logic Qt5::Core)
