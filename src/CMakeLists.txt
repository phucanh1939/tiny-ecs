add_library(tinyecs)

file(
    GLOB TINYECS_SOURCES
    CONFIGURE_DEPENDS
        *.cpp
)

target_sources(
    tinyecs
    PRIVATE
        ${TINYECS_SOURCES}
)

target_include_directories(
    tinyecs
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)