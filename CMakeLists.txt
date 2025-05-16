cmake_minimum_required(VERSION 3.10)
project(ParkingSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
# Qt path fix
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.0/mingw_64/lib/cmake")

# Qt setup
find_package(Qt6 REQUIRED COMPONENTS Widgets)

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
    vehicle.h
    IStrategy.h
    DefaultStrategy.h
    CustomRectangle.h
)

# Executable
add_executable(ParkingSimulator ${SOURCES} ${HEADERS})

# Link Qt6
target_link_libraries(ParkingSimulator Qt6::Widgets)
