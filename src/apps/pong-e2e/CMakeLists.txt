set(APP_NAME PongE2E)
set(APP_FOLDER_NAME pong-e2e)

set(SOURCES_E2E
        src/main.cpp
        ../pong/src/entities/GameObject.h
        ../pong/src/strategies/MinimalLoopStrategy.h
        src/PongE2ETest.h
        src/PongE2ETest.cpp
        src/tests/collision.test.cpp
        src/tests/paddle.test.cpp)

add_executable(${APP_NAME} ${SOURCES_E2E})

# Copy assets
file(COPY ../pong/assets DESTINATION ${CMAKE_BINARY_DIR}/src/apps/${APP_FOLDER_NAME})

# Link necessary libraries
target_link_libraries(${APP_NAME} Core
        Core
        gtest
        gmock
        gmock_main
        SDL2::SDL2
        )
