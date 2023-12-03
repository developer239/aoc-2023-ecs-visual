set(LIB_NAME Events)
set(LIB_NAME_TEST ${LIB_NAME}Test)

add_library(${LIB_NAME} STATIC
        ./events/Bus.h
        ./events/Bus.cpp
        ./events/Event.h
        )

target_include_directories(${LIB_NAME} INTERFACE .)

add_test(AllTests${LIB_NAME} ${LIB_NAME_TEST})

set(SOURCES_TEST
        ./events/Bus.h
        ./events/Bus.cpp
        ./events/Bus.test.cpp
        ./events/Event.h
        )

add_executable(${LIB_NAME_TEST} ${SOURCES_TEST})
target_link_libraries(${LIB_NAME_TEST} PRIVATE
        gtest
        gmock
        gmock_main)
