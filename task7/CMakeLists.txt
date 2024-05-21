add_executable(task7 main.cpp)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(task7 PRIVATE Qt5::Widgets)

set_target_properties(task7 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build"
    )


