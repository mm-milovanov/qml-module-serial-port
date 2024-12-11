cmake_minimum_required(VERSION 3.16)

project(qt-module-serial-port-lib LANGUAGES CXX)

find_package(Qt6 6.4 REQUIRED COMPONENTS Core Quick SerialPort Network)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

qt_add_library(${PROJECT_NAME} STATIC)

qt_add_qml_module(${PROJECT_NAME}
    URI SerialPortModule
    VERSION 1.0
    QML_FILES
        qml/SerialPort.qml
        
    SOURCES
    	include/serial_port.h
        src/serial_port.cpp
        
        include/gui/serial_port_gui.h
        src/gui/serial_port_gui.cpp
        
    RESOURCE_PREFIX
        /ext
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
