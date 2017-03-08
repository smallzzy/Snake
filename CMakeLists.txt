cmake_minimum_required(VERSION 3.0)
project(Snake)

# Cmake setting
set(CMAKE_CONFIGURATION_TYPES Debug;Release)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_SUPPRESS_REGENERATION true)

# compile setting
set(CMAKE_CXX_STANDARD 11)

#include path
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories("neurosky")

# link library path
if(WIN32)
link_directories("lib/win32/")
endif(WIN32)

#qt setting
if(WIN32)
set(CMAKE_PREFIX_PATH "C:\\Qt\\5.7\\msvc2015\\")
endif(WIN32)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5SerialPort REQUIRED)
set(CMAKE_AUTOMOC ON)

# qt file setting
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")
file(GLOB UIS "*.ui")
qt5_wrap_ui (UI_HEADERS ${UIS})

# compile
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_HEADERS})

# link
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::SerialPort thinkgear)

# dynamic link
if(WIN32)
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
# DEBUG
COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Cored.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Guid.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgetsd.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5SerialPortd.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_HOME_DIRECTORY}/lib/win32/thinkgear.dll ${CMAKE_BINARY_DIR}/Debug
# Output Message
COMMENT "Copying Qt binaries" VERBATIM)
endif(WIN32)
