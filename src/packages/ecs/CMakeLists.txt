set(LIB_NAME ECS)
set(LIB_NAME_TEST ${LIB_NAME}Test)

add_library(${LIB_NAME} STATIC
        ecs/Component.h
        ./ecs/Entity.cpp
        ./ecs/Entity.h
        ./ecs/IComponent.cpp
        ./ecs/IComponent.h
        ./ecs/IPool.h
        ./ecs/Pool.h
        ./ecs/Registry.h
        ./ecs/Signature.h
        ./ecs/System.h
        )

target_include_directories(${LIB_NAME} INTERFACE .)

add_test(AllTests${LIB_NAME} ${LIB_NAME_TEST})

set(SOURCES_TEST
        ecs/Component.h
        ./ecs/Entity.cpp
        ./ecs/Entity.h
        ./ecs/IComponent.cpp
        ./ecs/IComponent.h
        ./ecs/IPool.h
        ./ecs/Pool.h
        ./ecs/Registry.h
        ./ecs/Registry.test.cpp
        ./ecs/Signature.h
        ./ecs/System.h
        )

add_executable(${LIB_NAME_TEST} ${SOURCES_TEST})
target_link_libraries(${LIB_NAME_TEST} PRIVATE
        gtest
        gmock
        gmock_main)
