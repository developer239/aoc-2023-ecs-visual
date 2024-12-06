cmake_minimum_required(VERSION 3.10)
set(APP_NAME 2024day6)
set(APP_FOLDER_NAME 2024day6)

# Find OpenCV package
find_package(OpenCV REQUIRED)

set(SOURCES
        src/main.cpp
        src/entities/GameObject.h
        src/strategies/MinimalLoopStrategy.h
)


add_executable(${APP_NAME} ${SOURCES})

# Link OpenCV libraries
target_link_libraries(${APP_NAME} PRIVATE ${OpenCV_LIBS})

# Link Core library with consistent keyword
target_link_libraries(${APP_NAME} PRIVATE Core)

# Add OpenCV include directories
target_include_directories(${APP_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})

# Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/src/apps/${APP_FOLDER_NAME})
