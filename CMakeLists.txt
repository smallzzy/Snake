cmake_minimum_required(VERSION 3.0)
project(Snake)

# Cmake setting
set(CMAKE_CONFIGURATION_TYPES Debug;Release)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# default path setting
if(WIN32)
    if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
        set(QT_DEFAULT_PATH "C:\\Qt\\5.7\\msvc2015\\")
    else()
        set(QT_DEFAULT_PATH "C:\\Qt\\5.7\\msvc2015_64\\")
    endif()
    set(NEUROSKY_HEADER_PATH "neurosky")
elseif(APPLE)
    set(QT_DEFAULT_PATH "/Qt/5.8/clang_64")
    set(NEUROSKY_DEFAULT_PATH "")
endif(WIN32)

# configure setting
set(CMAKE_CXX_STANDARD 11)
set(QT_CURRENT_PATH ${QT_DEFAULT_PATH} CACHE STRING "The path to qt")
set(NEUROSKY_CURRENT_PATH ${NEUROSKY_DEFAULT_PATH} CACHE STRING "The path to neurosky lib/framework")

# neurosky header path

# neurosky library path
link_directories(NEUROSKY_CURRENT_PATH)
if(WIN32)
    include_directories(${NEUROSKY_HEADER_PATH})
elif(APPLE)
    find_library
    include_directories()
endif(WIN32)

#qt setting
set(CMAKE_PREFIX_PATH ${QT_CURRENT_PATH})
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
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::SerialPort )

# dynamic link / deploy setting
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
endif(WIN32)
