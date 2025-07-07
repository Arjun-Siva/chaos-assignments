add_library(common)

target_sources(common
    PRIVATE
        2dShapes/shapes.cpp
        core/vec3.cpp
        core/mat3.cpp
        core/triangle.cpp
        core/color.cpp
        core/ray.cpp
        core/camera.cpp
        core/mesh.cpp
        core/scene.cpp
        util/transform.h
        util/renderPPM.h
)
message("RapidJSON include path: ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson")

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/2dShapes
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/util
        ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson
)
