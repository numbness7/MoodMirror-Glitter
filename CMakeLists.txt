cmake_minimum_required(VERSION 3.28)
project(Glitter LANGUAGES C CXX)

# Vendor: GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(Glitter/Vendor/glfw)

# NONEED STR

# # Vendor: Assimp
# set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
# set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
# set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# add_subdirectory(Glitter/Vendor/assimp)
# 
# # Vendor: Bullet
# set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
# set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
# set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
# set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "" FORCE)
# set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
# add_subdirectory(Glitter/Vendor/bullet)

# NONEED END

# Vendor: GLAD (compiled as part of main target)
add_library(glad STATIC Glitter/Vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC Glitter/Vendor/glad/include/)

# Sources
set(PROJECT_HEADERS Glitter/Headers/glitter.hpp)
set(PROJECT_SOURCES Glitter/Sources/main.cpp)
set(PROJECT_SHADERS Glitter/Shaders/.gitignore)

# IDE source groups
source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})

# Main target
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_SHADERS}
    Glitter/Sources/shader.cpp
    Glitter/Headers/shader.hpp
    Glitter/Sources/stb_image.cpp
    Glitter/Headers/third_party.hpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    GLFW_INCLUDE_NONE
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

target_include_directories(${PROJECT_NAME} PRIVATE
    Glitter/Headers/
# NONEED STR
#    Glitter/Vendor/assimp/include/
#    Glitter/Vendor/bullet/src/
# NONEED END
    Glitter/Vendor/glm/
    Glitter/Vendor/stb/)

# Platform-specific link dependencies
if(NOT WIN32 AND NOT MSVC)
    set(PLATFORM_LIBRARIES dl)
endif()

#set(march native)
set(march x86-64)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
# NONEED STR
#    assimp
#    BulletDynamics
#    BulletCollision
#    LinearMath
# NONEED END
    ${PLATFORM_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

# Copy shaders to output directory after build
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Glitter/Shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMENT "Copying shaders to output directory")
