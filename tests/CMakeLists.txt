file(GLOB TEST_SOURCES
    CONFIGURE_DEPENDS
    "*.cpp"
)

add_executable(
    tinyecs-tests
    ${TEST_SOURCES}
)

target_link_libraries(
    tinyecs-tests
    PRIVATE
        tinyecs
)