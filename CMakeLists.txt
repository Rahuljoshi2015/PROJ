cmake_minimum_required(VERSION 3.10)
project(ParkingSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt path
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.0/mingw_64/lib/cmake")

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Source files
set(SOURCES
    main.cpp
    MainWindow.cpp
    Parking.cpp
    Vehicle.cpp
    DefaultStrategy.cpp
    CustomRectangle.cpp
)

# Header files
set(HEADERS
    MainWindow.h
    Parking.h
    Vehicle.h
    IStrategy.h
    DefaultStrategy.h
    CustomRectangle.h
)

# Executable
add_executable(ParkingSimulator ${SOURCES} ${HEADERS})

# Link Qt6 libraries
target_link_libraries(ParkingSimulator PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)