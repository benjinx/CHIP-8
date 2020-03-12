CMAKE_MINIMUM_REQUIRED(VERSION 3.12 FATAL_ERROR)

INCLUDE(ExternalProject)
PROJECT(ThirdParty)

# SDL2

SET(SDL2_ROOT       ${CMAKE_BINARY_DIR}/SDL2)
SET(SDL2_SOURCE_DIR ${CMAKE_BINARY_DIR}/SDL2_src)
SET(SDL2_BINARY_DIR ${CMAKE_BINARY_DIR}/SDL2_bin)

FIND_PACKAGE(SDL2 2.0.6 CONFIG QUIET)

IF(NOT SDL2_FOUND)
    ExternalProject_Add(
        ThirdParty_SDL2
        GIT_REPOSITORY      "https://github.com/spurious/SDL-mirror.git"
        GIT_TAG             "master"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          ${SDL2_SOURCE_DIR}
        BINARY_DIR          ${SDL2_BINARY_DIR}
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${SDL2_ROOT}
                            -DSDL_SHARED=OFF
                            -DSDL_STATIC=ON
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
        INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
    )
ENDIF()
