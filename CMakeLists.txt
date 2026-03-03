cmake_minimum_required(VERSION 3.16...4.2)

project(DoomLike VERSION 1.0.0 LANGUAGES CXX)

if(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "C:/msys64/mingw64")
    list(APPEND CMAKE_LIBRARY_PATH "C:/msys64/mingw64/lib")
    list(APPEND CMAKE_INCLUDE_PATH "C:/msys64/mingw64/include")
endif()

find_package(OpenGL REQUIRED)

find_package(GLEW QUIET)
if(NOT GLEW_FOUND)
    find_path(GLEW_INCLUDE_DIR
        NAMES GL/glew.h
        HINTS
            /usr/include
            /usr/local/include
            C:/msys64/mingw64/include
    )
    find_library(GLEW_LIBRARY
        NAMES GLEW glew32 glew32.dll
        HINTS
            /usr/lib
            /usr/local/lib
            C:/msys64/mingw64/lib
    )
    if(GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
        set(GLEW_FOUND TRUE)
        set(GLEW_INCLUDE_DIRS "${GLEW_INCLUDE_DIR}")
        set(GLEW_LIBRARIES "${GLEW_LIBRARY}")
        message(STATUS "Found GLEW: ${GLEW_LIBRARIES}")
    endif()
endif()

find_package(GLUT QUIET)
if(NOT GLUT_FOUND)
    find_path(GLUT_INCLUDE_DIR
        NAMES GL/glut.h GL/freeglut.h
        HINTS
            /usr/include
            /usr/local/include
            C:/msys64/mingw64/include
    )
    find_library(GLUT_LIBRARY
        NAMES glut freeglut freeglut.dll glut32
        HINTS
            /usr/lib
            /usr/local/lib
            C:/msys64/mingw64/lib
    )
    if(GLUT_INCLUDE_DIR AND GLUT_LIBRARY)
        set(GLUT_FOUND TRUE)
        set(GLUT_INCLUDE_DIRS "${GLUT_INCLUDE_DIR}")
        set(GLUT_LIBRARIES "${GLUT_LIBRARY}")
        message(STATUS "Found GLUT: ${GLUT_LIBRARIES}")
    endif()
endif()

find_package(OpenAL QUIET)
if(NOT OpenAL_FOUND)
    find_path(OPENAL_INCLUDE_DIR
        NAMES AL/al.h
        HINTS
            /usr/include
            /usr/local/include
            C:/msys64/mingw64/include
            "C:/Program Files/OpenAL 1.1 SDK/include"
    )
    find_library(OPENAL_LIBRARY
        NAMES openal OpenAL32 openal.dll libopenal.dll.a
        HINTS
            /usr/lib
            /usr/local/lib
            C:/msys64/mingw64/lib
            "C:/Program Files/OpenAL 1.1 SDK/libs/Win64"
    )
    if(OPENAL_INCLUDE_DIR AND OPENAL_LIBRARY)
        set(OpenAL_FOUND TRUE)
        set(OPENAL_INCLUDE_DIRS "${OPENAL_INCLUDE_DIR}")
        message(STATUS "Found OpenAL: ${OPENAL_LIBRARY}")
    endif()
endif()

if(NOT GLEW_FOUND)
    message(FATAL_ERROR "GLEW not found")
endif()
if(NOT GLUT_FOUND)
    message(FATAL_ERROR "GLUT not found")
endif()
if(NOT OpenAL_FOUND)
    message(FATAL_ERROR "OpenAL not found")
endif()

function(doomlike_add_interface_fallback target_name)
    set(options)
    set(one_value_args)
    set(multi_value_args INCLUDE_DIRS LINK_LIBS)
    cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    if(TARGET "${target_name}")
        return()
    endif()

    add_library("${target_name}" INTERFACE IMPORTED)
    if(ARG_INCLUDE_DIRS)
        set_property(TARGET "${target_name}" PROPERTY
            INTERFACE_INCLUDE_DIRECTORIES "${ARG_INCLUDE_DIRS}"
        )
    endif()
    if(ARG_LINK_LIBS)
        set_property(TARGET "${target_name}" PROPERTY
            INTERFACE_LINK_LIBRARIES "${ARG_LINK_LIBS}"
        )
    endif()
endfunction()

if(TARGET GLEW::GLEW)
    set(GLEW_TARGET GLEW::GLEW)
elseif(TARGET GLEW::glew)
    set(GLEW_TARGET GLEW::glew)
else()
    doomlike_add_interface_fallback(project_glew
        INCLUDE_DIRS "${GLEW_INCLUDE_DIRS}"
        LINK_LIBS "${GLEW_LIBRARIES}"
    )
    set(GLEW_TARGET project_glew)
endif()

if(TARGET GLUT::GLUT)
    set(GLUT_TARGET GLUT::GLUT)
else()
    doomlike_add_interface_fallback(project_glut
        INCLUDE_DIRS "${GLUT_INCLUDE_DIRS}"
        LINK_LIBS "${GLUT_LIBRARIES}"
    )
    set(GLUT_TARGET project_glut)
endif()

if(TARGET OpenAL::OpenAL)
    set(OPENAL_TARGET OpenAL::OpenAL)
else()
    doomlike_add_interface_fallback(project_openal
        INCLUDE_DIRS "${OPENAL_INCLUDE_DIRS}"
        LINK_LIBS "${OPENAL_LIBRARY}"
    )
    set(OPENAL_TARGET project_openal)
endif()

set(DOOMLIKE_SOURCES
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/src/audio/audio_engine.cpp
    ${CMAKE_SOURCE_DIR}/src/audio/audio_system.cpp
    ${CMAKE_SOURCE_DIR}/src/core/app.cpp
    ${CMAKE_SOURCE_DIR}/src/core/avatarEnemy.cpp
    ${CMAKE_SOURCE_DIR}/src/core/camera.cpp
    ${CMAKE_SOURCE_DIR}/src/core/entities.cpp
    ${CMAKE_SOURCE_DIR}/src/core/fps.cpp
    ${CMAKE_SOURCE_DIR}/src/core/game.cpp
    ${CMAKE_SOURCE_DIR}/src/core/light_system.cpp
    ${CMAKE_SOURCE_DIR}/src/core/movement.cpp
    ${CMAKE_SOURCE_DIR}/src/core/player.cpp
    ${CMAKE_SOURCE_DIR}/src/core/window.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/drawlevel.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/hud.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/lighting.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/menu.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/shader.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/skybox.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/texture.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/ui_text.cpp
    ${CMAKE_SOURCE_DIR}/src/input/input.cpp
    ${CMAKE_SOURCE_DIR}/src/input/keystate.cpp
    ${CMAKE_SOURCE_DIR}/src/level/level.cpp
    ${CMAKE_SOURCE_DIR}/src/level/level_validation.cpp
    ${CMAKE_SOURCE_DIR}/src/level/levelmetrics.cpp
    ${CMAKE_SOURCE_DIR}/src/level/maploader.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/assets.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/utils.cpp
)

set(DOOMLIKE_LEVEL_VALIDATOR_SOURCES
    ${CMAKE_SOURCE_DIR}/src/tools/level_validator_main.cpp
    ${CMAKE_SOURCE_DIR}/src/level/level.cpp
    ${CMAKE_SOURCE_DIR}/src/level/level_validation.cpp
    ${CMAKE_SOURCE_DIR}/src/level/levelmetrics.cpp
    ${CMAKE_SOURCE_DIR}/src/level/maploader.cpp
)

file(GLOB DOOMLIKE_LEVEL_MAP_FILES LIST_DIRECTORIES FALSE
    "${CMAKE_SOURCE_DIR}/maps/level*.txt"
)

set(DOOMLIKE_LEVEL_MAPS)
foreach(level_map IN LISTS DOOMLIKE_LEVEL_MAP_FILES)
    get_filename_component(level_name "${level_map}" NAME_WE)
    string(REGEX MATCH "level([0-9]+)$" _ "${level_name}")
    if(CMAKE_MATCH_1)
        list(APPEND DOOMLIKE_LEVEL_MAPS "${level_map}" "${CMAKE_MATCH_1}")
    endif()
endforeach()

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${DOOMLIKE_SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -g -O0>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /Zi /Od>
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    OpenGL::GLU
    ${GLEW_TARGET}
    ${GLUT_TARGET}
    ${OPENAL_TARGET}
)

add_executable(DoomLikeLevelValidator)

target_sources(DoomLikeLevelValidator PRIVATE ${DOOMLIKE_LEVEL_VALIDATOR_SOURCES})

target_compile_features(DoomLikeLevelValidator PRIVATE cxx_std_17)

target_compile_options(DoomLikeLevelValidator PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -g -O0>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /Zi /Od>
)

target_include_directories(DoomLikeLevelValidator PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

add_custom_target(validate_levels ALL
    COMMAND $<TARGET_FILE:DoomLikeLevelValidator> ${DOOMLIKE_LEVEL_MAPS}
    DEPENDS DoomLikeLevelValidator
    COMMENT "Validating level progression"
)

add_dependencies(${PROJECT_NAME} validate_levels)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

function(doomlike_copy_runtime_dll dll_name)
    if(NOT WIN32)
        return()
    endif()

    set(dll_path "${MINGW_BIN_DIR}/${dll_name}")
    if(EXISTS "${dll_path}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${dll_path}" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endif()
endfunction()

if(WIN32)
    set(MINGW_BIN_DIR "C:/msys64/mingw64/bin")

    doomlike_copy_runtime_dll("glew32.dll")
    doomlike_copy_runtime_dll("libfreeglut.dll")
    doomlike_copy_runtime_dll("libopenal-1.dll")
    doomlike_copy_runtime_dll("libgcc_s_seh-1.dll")
    doomlike_copy_runtime_dll("libstdc++-6.dll")
    doomlike_copy_runtime_dll("libwinpthread-1.dll")
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/maps $<TARGET_FILE_DIR:${PROJECT_NAME}>/maps
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMENT "Copying assets, maps, and shaders to build directory"
)
