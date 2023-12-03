set(LIB_NAME Logger)
set(LIB_NAME_TEST ${LIB_NAME}Test)

add_library(${LIB_NAME} STATIC
        ./logger/Logger.cpp
        ./logger/Logger.h
        )

target_include_directories(${LIB_NAME} INTERFACE .)

add_test(AllTests${LIB_NAME} ${LIB_NAME_TEST})

set(SOURCES_TEST
        ./logger/Logger.cpp
        ./logger/Logger.h
        ./logger/Logger.test.cpp)

add_executable(${LIB_NAME_TEST} ${SOURCES_TEST})
target_link_libraries(${LIB_NAME_TEST} PRIVATE
        gtest
        gmock
        gmock_main)
