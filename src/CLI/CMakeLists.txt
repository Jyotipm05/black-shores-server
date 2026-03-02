set(CMAKE_CXX_SCAN_FOR_MODULES ON)

find_package(Drogon CONFIG REQUIRED)

add_library(CLI)

target_link_libraries(CLI PRIVATE
        Drogon::Drogon
)

target_sources(CLI PUBLIC
        FILE_SET CXX_MODULES FILES
        CLI.ixx
        Data_Manager.ixx
)
