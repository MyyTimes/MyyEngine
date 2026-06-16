cmake_minimum_required(VERSION 3.16)
project(GameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 and Box2D packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

find_package(box2d QUIET)
if(NOT box2d_FOUND)
    pkg_check_modules(BOX2D REQUIRED box2d)
endif()

# Collect all source files
file(GLOB_RECURSE SOURCES
    "GameEngine/Application/*.cpp"
    "GameEngine/Engine/**/*.cpp"
)

file(GLOB_RECURSE HEADERS
    "GameEngine/Engine/**/*.h"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/GameEngine
    ${CMAKE_SOURCE_DIR}/GameEngine/Engine
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)

if(box2d_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE box2d::box2d)
else()
    target_include_directories(${PROJECT_NAME} PRIVATE ${BOX2D_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${BOX2D_LIBRARIES})
endif()

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/GameEngine/Assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets"
    COMMENT "Copying Assets to build directory..."
)
