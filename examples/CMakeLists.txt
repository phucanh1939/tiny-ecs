add_executable(
    tinyecs-example
    main.cpp
)

target_link_libraries(
    tinyecs-example
    PRIVATE
        tinyecs
)
