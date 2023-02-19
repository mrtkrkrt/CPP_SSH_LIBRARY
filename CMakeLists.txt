cmake_minimum_required(VERSION 3.14)

project(SSH LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/)

find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED) #if error run: sudo apt-get install libgl1-mesa-dev
find_package(libssh REQUIRED)

set(PROJECT_SOURCES
    ssh.cpp
)

# Set some Win32 Specific Settings
IF(WIN32)
SET(SSH_TYPE WIN32)
ENDIF(WIN32)
# Set some Apple MacOS Specific settings
IF (APPLE)
SET(SSH_TYPE MACOSX_BUNDLE)
ENDIF (APPLE)

add_executable(SSH ${SSH_TYPE} ${PROJECT_SOURCES})

target_link_libraries(SSH PRIVATE ssh)