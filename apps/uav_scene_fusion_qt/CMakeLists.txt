cmake_minimum_required(VERSION 3.16)
project(UavSceneFusionQt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets Network)

# Assimp - 摄影测量模型加载
find_package(assimp QUIET)
if(NOT assimp_FOUND)
    # 尝试手动指定路径（从源码编译安装的位置）
    find_library(ASSIMP_LIB NAMES assimp HINTS /usr/local/lib /usr/lib)
    find_path(ASSIMP_INCLUDE NAMES assimp/Importer.hpp HINTS /usr/local/include /usr/include)
    if(ASSIMP_LIB AND ASSIMP_INCLUDE)
        set(assimp_FOUND TRUE)
        message(STATUS "Found Assimp manually: ${ASSIMP_LIB}")
    else()
        message(WARNING "Assimp not found - OBJ model loading disabled")
    endif()
endif()

set(SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
    RenderWidget.cpp
    RenderWidget.h
    telemetry/UavState.h
    telemetry/UdpJsonReceiver.cpp
    telemetry/UdpJsonReceiver.h
    render/UavRenderer.cpp
    render/UavRenderer.h
    render/TrajectoryRenderer.cpp
    render/TrajectoryRenderer.h
    scene/SimpleScene.cpp
    scene/SimpleScene.h
    geo/CoordinateManager.cpp
    geo/CoordinateManager.h
    render/ObjModelRenderer.cpp
    render/ObjModelRenderer.h
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    Qt6::Network
)

# 链接 Assimp
if(assimp_FOUND)
    if(TARGET assimp::assimp)
        target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)
    elseif(ASSIMP_LIB)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${ASSIMP_LIB})
        target_include_directories(${PROJECT_NAME} PRIVATE ${ASSIMP_INCLUDE})
    endif()
    target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_ASSIMP)
endif()
