set(APP_NAME AppPong)
set(APP_FOLDER_NAME pong)

set(APP_NAME_TEST ${LIB_NAME}Test)

set(SOURCES
        src/main.cpp
        src/entities/GameObject.h
        src/strategies/MinimalLoopStrategy.h
        )

add_executable(${APP_NAME} ${SOURCES})

# Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/src/apps/${APP_FOLDER_NAME})

target_link_libraries(${APP_NAME} Core)

#
# Tests

add_test(AllTests${LIB_NAME} ${APP_NAME_TEST})

set(SOURCES_TEST
        src/entities/GameObject.h
        src/strategies/MinimalLoopStrategy.h
        src/entities/GameObject.test.cpp
        src/strategies/MinimalLoopStrategy.test.cpp)

add_executable(${APP_NAME_TEST} ${SOURCES_TEST})

target_link_libraries(${APP_NAME_TEST} PRIVATE
        gtest
        gmock
        gmock_main
        Core
        SDL2::SDL2
        )
