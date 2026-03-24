cmake_minimum_required(VERSION 4.0)
project(black-shores-server VERSION 1.0.0 LANGUAGES CXX)

if (WIN64)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
elseif (UNIX)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif ()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)


find_package(Drogon CONFIG REQUIRED)

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Drogon::Drogon
        src
)