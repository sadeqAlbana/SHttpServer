cmake_minimum_required(VERSION 3.16)

find_package(Qt6  COMPONENTS Core REQUIRED)
find_package(Qt6  COMPONENTS Network REQUIRED)
add_library(SHttpServer STATIC
    abstractmiddleware.h abstractmiddleware.cpp
	controller.h controller.cpp
    router.h router.cpp
shttprequest.h shttprequest.cpp
shttpresponse.h shttpresponse.cpp
shttpserver.h shttpserver.cpp
ssockethandler.h ssockethandler.cpp

)

target_link_libraries(SHttpServer PRIVATE Qt6::Core)
target_link_libraries(SHttpServer PRIVATE Qt6::Network)

target_include_directories(SHttpServer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
