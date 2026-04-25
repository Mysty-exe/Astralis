cmake_minimum_required(VERSION 3.20)

project(Astralis)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})

# Collect source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Add icon resource
set(APP_ICON icon.rc)

# Single executable
add_executable(Astralis
    ${SOURCES}
    ${APP_ICON}
)

# Include directories
target_include_directories(Astralis PRIVATE
    include
    libs
    libs/SDL2/include
    libs/SDL2_image/include
    libs/SDL2_ttf/include
    libs/SDL2_mixer/include
)

# SDL2
target_link_libraries(Astralis PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2main.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_image/lib/x64/SDL2_image.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_ttf/lib/x64/SDL2_ttf.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_mixer/lib/x64/SDL2_mixer.lib"
)

# Copy DLLs
add_custom_command(TARGET Astralis POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.dll"
    $<TARGET_FILE_DIR:Astralis>
)

add_custom_command(TARGET Astralis POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_image/lib/x64/SDL2_image.dll"
    $<TARGET_FILE_DIR:Astralis>
)

add_custom_command(TARGET Astralis POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_ttf/lib/x64/SDL2_ttf.dll"
    $<TARGET_FILE_DIR:Astralis>
)

add_custom_command(TARGET Astralis POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_mixer/lib/x64/SDL2_mixer.dll"
    $<TARGET_FILE_DIR:Astralis>
)