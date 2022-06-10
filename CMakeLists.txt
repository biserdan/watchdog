cmake_minimum_required(VERSION 3.16)
project(BA)

set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(BA
        Client/client.cpp Client/Gamepad.cpp Client/Gamepad.h Client/UDP_Client.cpp Client/UDP_Client.h)
add_executable(Server
        Server/server.cpp Server/Controller.cpp Server/Controller.h Server/UDP_Server.cpp Server/UDP_Server.h)

find_package(JetsonGPIO)
target_link_libraries(Server JetsonGPIO::JetsonGPIO)
target_link_libraries(BA SDL2)
