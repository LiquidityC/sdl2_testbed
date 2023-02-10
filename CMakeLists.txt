cmake_minimum_required(VERSION 3.7)

project(testbed LANGUAGES C VERSION 0.1.0)

# Compile and link flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

# Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

if (LOCAL_SDL)
	message("Using local SDL library")
	add_subdirectory(SDL)
	add_definitions(-DSDL_NEXT)
	set(SDL_LIBRARIES SDL3 SDL3_Headers)
else ()
	message("Using system installed SDL library")
	include(FindSDL2)
	set(SDL_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
	set(SDL_LIBRARIES ${SDL2_LIBRARIES})
endif ()

# Environment defines
if (CMAKE_BUILD_TYPE MATCHES "Debug")
	set(DEBUG 1)
endif ()

file(GLOB SOURCE_FILES src/*.c)

foreach(F IN LISTS SOURCE_FILES)
	get_filename_component(EXENAME ${F} NAME_WE)
	add_executable(${EXENAME} ${F})
	target_include_directories(${EXENAME} PRIVATE ${SDL_INCLUDE_DIRS})
	target_link_libraries(${EXENAME} ${SDL_LIBRARIES})
endforeach ()
