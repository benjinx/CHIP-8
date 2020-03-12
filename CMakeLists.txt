CMAKE_MINIMUM_REQUIRED(VERSION 3.12 FATAL_ERROR)

###
### Project
###

PROJECT(CHIP-8)

###
### Options
###
OPTION(BUILD_THIRDPARTY "Build Missing ThirdParty Dependencies" ON)

###
### Configuration
###

SET(_TARGET "CHIP-8")

# Glob
FILE(GLOB_RECURSE
	_SOURCES
	"${CMAKE_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_SOURCE_DIR}/src/*.c"
	"${CMAKE_SOURCE_DIR}/src/*.hpp"
	"${CMAKE_SOURCE_DIR}/src/*.h"
)

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Folders to filters
FOREACH(file IN ITEMS ${_ENGINE_INCLUDES} ${_ENGINE_SOURCES})
    FILE(RELATIVE_PATH file_path ${CMAKE_SOURCE_DIR} ${file})
    GET_FILENAME_COMPONENT(file_path ${file_path} DIRECTORY)
    FILE(TO_NATIVE_PATH ${file_path} file_path)
    SOURCE_GROUP(${file_path} FILES ${file})
ENDFOREACH()

ADD_EXECUTABLE(
	${_TARGET}
	${_SOURCES}
)

TARGET_COMPILE_DEFINITIONS(
	${_TARGET}
	PUBLIC
	$<$<C_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

###
### Third Party Dependencies
###
IF(BUILD_THIRDPARTY)
	SET(THIRDPARTY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty)
	SET(THIRDPARTY_BINARY_DIR ${CMAKE_BINARY_DIR}/thirdparty)

	FILE(MAKE_DIRECTORY ${THIRDPARTY_BINARY_DIR})

	# Set _ROOT variables for config-based packages
	SET(SDL2_ROOT ${THIRDPARTY_BINARY_DIR}/SDL2)

	# Set generators the same as ours
	EXECUTE_PROCESS(
		COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
			-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
			${THIRDPARTY_SOURCE_DIR}
			WORKING_DIRECTORY ${THIRDPARTY_BINARY_DIR}
		RESULT_VARIABLE _result
	)

	IF(_result)
		MESSAGE("${_result}")
		MESSAGE(FATAL_ERROR "Failed to configure thirdparty projects.")
	ENDIF()

	# Build the thirdparty libraries
	SET(_THIRDPARTY_BUILD_COMMAND ${CMAKE_COMMAND} --build . )
	IF(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
		SET(_THIRDPARTY_BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -l )
	ENDIF()

	EXECUTE_PROCESS(
		COMMAND ${_THIRDPARTY_BUILD_COMMAND}
		WORKING_DIRECTORY ${THIRDPARTY_BINARY_DIR}
		RESULT_VARIABLE _result
	)

	IF(_result)
		MESSAGE("${_result}")
		MESSAGE(FATAL_ERROR "Failed to build thirdparty projects")
	ENDIF()
ENDIF()

# Checking to make sure we have it.
FIND_PACKAGE(SDL2 2.0.6 CONFIG REQUIRED)

TARGET_LINK_LIBRARIES(
	${_TARGET}
	PUBLIC
		SDL2::SDL2-static
)

IF(MSVC)
	TARGET_LINK_LIBRARIES(
		${_TARGET}
		PUBLIC
			SDL2::SDL2main
    )
ENDIF()
