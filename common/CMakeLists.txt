add_library(common)

target_sources(common
    PRIVATE
        2dShapes/shapes.cpp
        core/vec3.cpp
)

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/2dShapes
        ${CMAKE_CURRENT_SOURCE_DIR}/core
)
