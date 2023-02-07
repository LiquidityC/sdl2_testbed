cmake_minimum_required(VERSION 3.7)

project(testbed LANGUAGES C VERSION 0.1.0)

# Compile and link flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

# Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include(FindSDL2)
include(ClangFormat)

# Environment defines
if (CMAKE_BUILD_TYPE MATCHES "Debug")
	set(DEBUG 1)
endif ()

add_executable(${PROJECT_NAME}
	src/main.c
	)

target_include_directories(${PROJECT_NAME}
	PRIVATE
	inc
	${SDL2_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}/include
	)

target_clangformat_setup(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
	${SDL2_LIBRARIES}
	)
