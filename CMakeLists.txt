cmake_minimum_required(VERSION 4.0.0)
project(black-shores-server VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(Drogon CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/IpCollector.hpp
        src/dotenv.hpp
        src/create_cert.cpp
        src/create_cert.hpp
        src/controllers/home.cpp
        src/controllers/home.hpp
        src/controllers/RootCtrl.cpp
        src/controllers/RootCtrl.hpp
        src/WebSockets/ChatWS.cpp
        src/WebSockets/ChatWS.h
        src/controllers/CritExit.cpp
        src/controllers/CritExit.hpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Drogon::Drogon
)