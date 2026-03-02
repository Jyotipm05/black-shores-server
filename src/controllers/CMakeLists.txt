find_package(Drogon CONFIG REQUIRED)

add_library(controllers STATIC
        CritExit.cpp
        CritExit.hpp
        home.cpp
        home.hpp
        PathManager.hpp
        RootCtrl.cpp
        RootCtrl.hpp
        stream_test.cpp
        stream_test.hpp
)

target_link_libraries(controllers PRIVATE
        Drogon::Drogon
)
