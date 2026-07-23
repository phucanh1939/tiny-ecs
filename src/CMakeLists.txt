add_library(tinyecs)

target_sources(
    tinyecs
    PRIVATE
        World.cpp
)

target_include_directories(
    tinyecs
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
