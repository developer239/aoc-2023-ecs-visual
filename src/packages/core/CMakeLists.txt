set(LIB_NAME Core)

add_library(${LIB_NAME} STATIC
        core/AssetStore.cpp
        core/AssetStore.h
        core/Loop.cpp
        core/Loop.h
        core/Renderer.cpp
        core/Renderer.h
        core/IStrategy.h
        core/Window.cpp
        core/Window.h)

target_include_directories(${LIB_NAME} INTERFACE .)

target_link_libraries(${LIB_NAME} PUBLIC
        SDL2::SDL2
        SDL2::TTF
        SDL2::Image
        SDL2::Mixer)
target_link_libraries(${LIB_NAME} PRIVATE Logger)
