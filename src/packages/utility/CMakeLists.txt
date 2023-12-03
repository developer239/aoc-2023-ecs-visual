set(LIB_NAME Utility)

add_library(${LIB_NAME} STATIC
        ./utility/Delay.h
        ./utility/Delay.cpp
        )

target_include_directories(${LIB_NAME} INTERFACE .)

target_link_libraries(${LIB_NAME} PRIVATE
        Core
        Devices
        )
