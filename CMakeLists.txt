cmake_minimum_required(VERSION 3.31.0)
project(black-shores-server VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(Drogon CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/IpCollector.cpp
        src/IpCollector.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Drogon::Drogon
)