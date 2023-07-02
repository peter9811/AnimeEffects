cmake_minimum_required(VERSION 3.16)
project(AnimeEffects VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

find_package(Qt5 REQUIRED COMPONENTS Core Gui OpenGL Xml OPTIONAL_COMPONENTS Widgets)


add_subdirectory(util)
add_subdirectory(thr)
add_subdirectory(cmnd)
add_subdirectory(gl)
add_subdirectory(img)
add_subdirectory(core)
add_subdirectory(ctrl)
add_subdirectory(gui)
