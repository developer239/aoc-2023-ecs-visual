set(LIB_NAME CoreImgui)

add_library(${LIB_NAME} STATIC
        core-imgui/Strategy.cpp
        core-imgui/Strategy.h)

target_include_directories(${LIB_NAME} INTERFACE .)

target_link_libraries(${LIB_NAME} PRIVATE Core)

target_link_libraries(${LIB_NAME} PUBLIC Imgui)