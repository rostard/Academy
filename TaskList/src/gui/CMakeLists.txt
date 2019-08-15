include(GenerateExportHeader)

find_package(Qt5Core)
find_package(Qt5Widgets)

file(GLOB_RECURSE lib_includes "include/*.h")
file(GLOB_RECURSE lib_sources "src/*.cpp" "src/*.h")
file(GLOB_RECURSE lib_ui "ui_forms/*.ui")

set(CMAKE_AUTOUIC_SEARCH_PATHS  "ui_forms")

add_library(gui SHARED ${lib_includes} ${lib_sources} ${lib_ui})

generate_export_header(gui EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/exports/gui_export.h")

target_include_directories(gui PUBLIC "include" "src" "${CMAKE_BINARY_DIR}/exports")
target_link_libraries(gui logic Qt5::Core Qt5::Widgets)
