add_library(common)

target_sources(common
    PRIVATE
        2dShapes/shapes.cpp
        linalg/vec3.cpp
        linalg/mat3.cpp
        core/triangle.cpp
        core/color.cpp
        core/ray.h
        core/camera.cpp
        core/mesh.cpp
        core/scene.cpp
        core/material.cpp
        core/light.cpp
        util/transform.h
        util/renderPPM.h
)

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/2dShapes
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/linalg
        ${CMAKE_CURRENT_SOURCE_DIR}/util
        ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson
)
