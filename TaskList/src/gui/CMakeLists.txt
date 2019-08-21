include(GenerateExportHeader)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE lib_includes "include/*.h")
file(GLOB_RECURSE lib_sources "src/*.cpp" "src/*.h")
file(GLOB_RECURSE lib_ui "ui/*.ui")

set(qrc_path "ui/qml.qrc")

set(CMAKE_AUTOUIC_SEARCH_PATHS  "ui")

add_library(gui SHARED ${lib_includes} ${lib_sources} ${qrc_path})

generate_export_header(gui EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/exports/gui_export.h")

target_include_directories(gui PUBLIC "include" "src" "${CMAKE_BINARY_DIR}/exports")
target_link_libraries(gui PUBLIC Qt5::Core Qt5::Quick)
