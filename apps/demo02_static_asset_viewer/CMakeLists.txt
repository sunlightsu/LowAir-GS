cmake_minimum_required(VERSION 3.16)
project(Demo02StaticAssetViewer VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ─── Qt6 ───────────────────────────────────────────────────────────────────
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)

# ─── Assimp ────────────────────────────────────────────────────────────────
# Demo-02 uses Assimp for external model loading (OBJ / PLY / GLB).
# Demo-01 does NOT use Assimp — the two CMakeLists.txt are independent.
find_package(assimp REQUIRED)

# ─── OpenGL ────────────────────────────────────────────────────────────────
find_package(OpenGL REQUIRED)

# ─── Sources ───────────────────────────────────────────────────────────────
set(SOURCES
    main.cpp
    MainWindow.cpp
    RenderWidget.cpp

    asset/ModelAsset.cpp
    asset/ModelLoader.cpp
    asset/AssimpModelLoader.cpp

    render/MeshRenderer.cpp
    render/GridRenderer.cpp
    render/AxisRenderer.cpp
    render/BoundingBoxRenderer.cpp

    camera/OrbitCamera.cpp
)

set(HEADERS
    MainWindow.h
    RenderWidget.h

    asset/ModelAsset.h
    asset/ModelLoader.h
    asset/AssimpModelLoader.h

    render/MeshRenderer.h
    render/GridRenderer.h
    render/AxisRenderer.h
    render/BoundingBoxRenderer.h

    camera/OrbitCamera.h
)

# ─── Executable ────────────────────────────────────────────────────────────
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ASSIMP_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    OpenGL::GL
    assimp
)

# ─── Compiler flags ────────────────────────────────────────────────────────
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()
