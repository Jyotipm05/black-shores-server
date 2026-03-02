find_package(Drogon CONFIG REQUIRED)

add_library(WebSockets STATIC
        ChatWS.cpp
        ChatWS.hpp
)

target_link_libraries(WebSockets PRIVATE
        Drogon::Drogon
)
