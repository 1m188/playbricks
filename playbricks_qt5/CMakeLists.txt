cmake_minimum_required(VERSION 3.15)
project(playbricks_qt5)

find_package(Qt5 REQUIRED Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

aux_source_directory(. sources)
add_executable(${PROJECT_NAME} ${sources})
target_include_directories(${PROJECT_NAME} PRIVATE ./scene ./sprite ./game)
file(GLOB srcs ./scene/*.cpp ./sprite/*.cpp ./game/*.cpp)
target_sources(${PROJECT_NAME} PRIVATE ./playbricks_qt5.qrc ${srcs})
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Release>:RELEASE>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/subsystem:windows /entry:mainCRTStartup")
endif()