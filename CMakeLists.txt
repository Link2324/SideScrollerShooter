# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(SideScroller)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/game)

# Add the executable
add_executable(SideScroller 
    src/main.c 
    src/sound.c 
    src/draw.c
    src/init.c
    src/input.c
    src/stage.c
    src/util.c
)

# Find SDL2 using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

# Include SDL2 and SDL2_mixer headers
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# Link SDL2 and SDL2_mixer libraries
target_link_libraries(SideScroller ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

# Directories containing resources
set(SOURCE_GRAPHICS_DIR "${CMAKE_SOURCE_DIR}/graphics")
set(SOURCE_MUSIC_DIR "${CMAKE_SOURCE_DIR}/music")
set(SOURCE_SOUND_DIR "${CMAKE_SOURCE_DIR}/sound")

# Destination directories in the build folder
set(DEST_GRAPHICS_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/graphics")
set(DEST_MUSIC_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/music")
set(DEST_SOUND_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sound")

# Custom targets to copy the directories
add_custom_target(CopyResources ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_GRAPHICS_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_GRAPHICS_DIR} ${DEST_GRAPHICS_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_MUSIC_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_MUSIC_DIR} ${DEST_MUSIC_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_SOUND_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_SOUND_DIR} ${DEST_SOUND_DIR}
    COMMENT "Copying resources (graphics, music, sound) to the build directory"
)

# Ensure resources are copied before building the executable
add_dependencies(SideScroller CopyResources)
