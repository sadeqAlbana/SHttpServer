cmake_minimum_required(VERSION 3.16)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)

add_library(SHttpServer STATIC
    abstractmiddleware.h abstractmiddleware.cpp
    controller.h controller.cpp
    router.h router.cpp
    shttprequest.h shttprequest.cpp
    shttpresponse.h shttpresponse.cpp
    shttpserver.h shttpserver.cpp
    ssockethandler.h ssockethandler.cpp
    httpcommon.h httpcommon.cpp

)

target_link_libraries(SHttpServer PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

target_include_directories(SHttpServer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
