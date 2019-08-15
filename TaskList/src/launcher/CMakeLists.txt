find_package(Qt5Core)
find_package(Qt5Widgets)

file(GLOB_RECURSE lib_includes "include/*.h")
file(GLOB_RECURSE lib_sources "src/*.cpp")

#set(QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ui/modules" CACHE STRING "path to force Qt Creator to detect modules in Resources" FORCE)

#qt5_add_resources(RESOURCES "ui/launcher.qrc")

add_executable(launcher WIN32 ${lib_includes} ${lib_sources} ${RESOURCES})
target_link_libraries(launcher Qt5::Core Qt5::Widgets logic gui)
target_include_directories(launcher PUBLIC "src" "${CMAKE_BINARY_DIR}/exports")
