find_package(Drogon CONFIG REQUIRED)

add_subdirectory(controllers)
add_subdirectory(WebSockets)
add_subdirectory(CLI)

add_library(src STATIC
        create_cert.cpp
        create_cert.hpp
        dotenv.hpp
        GetIPv4IPv6.hpp
        IpCollector.hpp
        pathFinder.hpp
        IPvalidator.hpp
)

add_executable(black-shores-server main.cpp)

target_link_libraries(src PRIVATE
        Drogon::Drogon
        controllers
        WebSockets
        CLI
)