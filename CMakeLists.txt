cmake_minimum_required(VERSION 3.16)

project(io-port-lib LANGUAGES CXX)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core SerialPort Network)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(UTILS_PING_SRC ping_windows.cpp)
endif(WIN32)

if(UNIX)
    set(UTILS_PING_SRC ping_linux.cpp)
endif(UNIX)

qt_add_library(${PROJECT_NAME} STATIC)

qt_add_qml_module(${PROJECT_NAME}
    URI SerialPortModule
    VERSION 1.0
    QML_FILES
        qml/SerialPort.qml
        qml/UdpPort.qml
        
    SOURCES
    	include/serial_port.h
        src/serial_port.cpp
        include/udp_port.h
        src/udp_port.cpp
        
        include/gui/serial_port_gui.h
        src/gui/serial_port_gui.cpp
        include/gui/udp_port_gui.h
        src/gui/udp_port_gui.cpp
        
    RESOURCE_PREFIX
        /module/io-port
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
                        Qt${QT_VERSION_MAJOR}::Core
                        Qt${QT_VERSION_MAJOR}::SerialPort
                        Qt${QT_VERSION_MAJOR}::Network)
