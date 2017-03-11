cmake_minimum_required(VERSION 3.0)
project(Snake)

# Cmake setting
set(CMAKE_CONFIGURATION_TYPES Debug;Release)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
cmake_policy(SET CMP0054 NEW)

# print
MESSAGE( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER} )
MESSAGE( STATUS "CMAKE_GENERATOR: " ${CMAKE_GENERATOR} )

# default path setting
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(QT_DEFAULT_PATH "C:\\Qt\\5.7\\msvc2015\\")
    else()
        set(QT_DEFAULT_PATH "C:\\Qt\\5.7\\msvc2015_64\\")
    endif()
    set(NEUROSKY_HEADER_PATH "neurosky")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(QT_DEFAULT_PATH "/Qt/5.8/clang_64")
endif()

# configure setting
set(CMAKE_CXX_STANDARD 11)
set(QT_CURRENT_PATH ${QT_DEFAULT_PATH} CACHE PATH "Qt kit")
set(NEUROSKY_STREAM_PATH "" CACHE PATH "Neurosky Stream lib/framework")
set(NEUROSKY_ALGO_PATH "" CACHE PATH "Neurosky Algo lib/framework")

# neurosky setting
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${NEUROSKY_STREAM_PATH})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        find_library(NEUROSKY_STREAM
            NAMES thinkgear
            PATH_SUFFIXES "win32")
    else()
        find_library(NEUROSKY_STREAM
            NAMES thinkgear64
            PATH_SUFFIXES "x64")
    endif()
    include_directories(${NEUROSKY_HEADER_PATH})
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
find_library(NEUROSKY_STREAM AlgoSdk)
    include_directories()
endif()

# qt setting
set(CMAKE_PREFIX_PATH ${QT_CURRENT_PATH})
find_package(Qt5Widgets REQUIRED)
find_package(Qt5SerialPort REQUIRED)
set(CMAKE_AUTOMOC ON)

# source file setting
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")
file(GLOB UIS "*.ui")
qt5_wrap_ui (UI_HEADERS ${UIS})

# compile
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_HEADERS})

# link
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::SerialPort ${NEUROSKY_STREAM})

# dynamic link / deploy setting
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
# DEBUG
COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Cored.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Guid.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgetsd.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5SerialPortd.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${NEUROSKY_STREAM_PATH}/win32/thinkgear.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${NEUROSKY_ALGO_PATH}/AlgoSdkDll.dll ${CMAKE_BINARY_DIR}/Debug
# Output Message
COMMENT "Copying Qt binaries" VERBATIM)


else()
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Cored.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Guid.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgetsd.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5SerialPortd.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${NEUROSKY_STREAM_PATH}/x64/thinkgear64.dll ${CMAKE_BINARY_DIR}/Debug
COMMAND ${CMAKE_COMMAND} -E copy ${NEUROSKY_ALGO_PATH}/AlgoSdkDll64.dll ${CMAKE_BINARY_DIR}/Debug
COMMENT "Copying Neuosky binaries" VERBATIM)
endif()
elseif(APPLE)
    #    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
    # DEBUG
    #mkdir Snake.app/Contents/Frameworks
    #cp -R /path/to/Qt/lib/QtCore.framework
    #        plugandpaint.app/Contents/Frameworks
    #cp -R /path/to/Qt/lib/QtGui.framework
    #       plugandpaint.app/Contents/Frameworks
    #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Cored.dll ${CMAKE_BINARY_DIR}/Debug
    #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Guid.dll ${CMAKE_BINARY_DIR}/Debug
    #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgetsd.dll ${CMAKE_BINARY_DIR}/Debug
    #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5SerialPortd.dll ${CMAKE_BINARY_DIR}/Debug
    #COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_HOME_DIRECTORY}/lib/win32/thinkgear.dll ${CMAKE_BINARY_DIR}/Debug
    # Output Message
    # COMMENT "Copying Qt binaries" VERBATIM)
endif()
