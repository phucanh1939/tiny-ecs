add_executable(
    tinyecs-tests
    main.cpp
)

target_link_libraries(
    tinyecs-tests
    PRIVATE
        tinyecs
)
