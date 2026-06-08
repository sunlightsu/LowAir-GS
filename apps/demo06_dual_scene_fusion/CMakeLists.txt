cmake_minimum_required(VERSION 3.16)
project(Demo06DualSceneFusion VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)
find_package(assimp REQUIRED)
find_package(OpenGL REQUIRED)

set(SOURCES
    main.cpp
    MainWindow.cpp
    RenderWidget.cpp
    mesh/AssimpMeshLoader.cpp
    mesh/MeshRenderer.cpp
    gaussian/GaussianCloud.cpp
    gaussian/GaussianPlyLoader.cpp
    gaussian/GaussianBillboardRenderer.cpp
    fusion/FusionConfig.cpp
    fusion/FusionTransform.cpp
    fusion/FusionLayerController.cpp
    fusion/FusionMetrics.cpp
    render/GridRenderer.cpp
    render/AxisRenderer.cpp
    render/BoundingBoxRenderer.cpp
    camera/OrbitCamera.cpp
    perf/FpsCounter.cpp
)

set(HEADERS
    MainWindow.h
    RenderWidget.h
    mesh/MeshAsset.h
    mesh/MeshLoader.h
    mesh/AssimpMeshLoader.h
    mesh/MeshRenderer.h
    gaussian/GaussianPoint.h
    gaussian/GaussianCloud.h
    gaussian/GaussianPlyLoader.h
    gaussian/GaussianBillboardRenderer.h
    fusion/FusionConfig.h
    fusion/FusionTransform.h
    fusion/FusionLayerController.h
    fusion/FusionMetrics.h
    render/GridRenderer.h
    render/AxisRenderer.h
    render/BoundingBoxRenderer.h
    camera/OrbitCamera.h
    perf/FpsCounter.h
)

add_executable(Demo06DualSceneFusion ${SOURCES} ${HEADERS})

target_link_libraries(Demo06DualSceneFusion
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
        assimp::assimp
        OpenGL::GL
)

target_include_directories(Demo06DualSceneFusion
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
