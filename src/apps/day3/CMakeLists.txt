set(APP_NAME Day3)
set(APP_FOLDER_NAME day3)

set(APP_NAME_TEST ${LIB_NAME}Test)

set(SOURCES
        src/main.cpp
        src/common/Vec2.h
        src/components/BoxColliderComponent.h
        src/components/KeyboardControlledComponent.h
        src/components/RigidBodyComponent.h
        src/events/CollisionEvent.h
        src/events/KeyPressedEvent.h
        src/systems/CollisionSystem.h
        src/systems/KeyboardControlSystem.h
        src/systems/RenderRigidBodiesSystem.h
        src/strategies/ECSStrategy.h
        src/services/PuzzleInputParser.h
        src/components/TextComponent.h
        src/systems/RenderTextSystem.h
        src/systems/RenderGridSystem.h
        src/systems/RenderColliderSystem.h
        src/systems/PuzzleSolverSystem.h
        src/components/CameraComponent.h src/systems/CameraSystem.h)

add_executable(${APP_NAME} ${SOURCES})

# Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/src/apps/${APP_FOLDER_NAME})

target_link_libraries(${APP_NAME} Core)
target_link_libraries(${APP_NAME} ECS)
target_link_libraries(${APP_NAME} Events)
target_link_libraries(${APP_NAME} Logger)
