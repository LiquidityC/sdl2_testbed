cmake_minimum_required(VERSION 3.7)

project(testbed LANGUAGES C VERSION 0.1.0)

# Compile and link flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

# Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

if (GIT_SDL)
	message("Using GIT SDL library")
	add_subdirectory(SDL)
	add_definitions(-DSDL_NEXT -DSDL_GIT)
	set(SDL_LIBRARIES SDL3-shared SDL3_Headers)
elseif (LOCAL_SDL)
	message("Using local SDL library")
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../SDL ${CMAKE_CURRENT_BINARY_DIR}/SDL_LOCAL)
	add_definitions(-DSDL_NEXT -DSDL_LOCAL)
	set(SDL_LIBRARIES SDL3-shared SDL3_Headers)
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

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS src/*.c)

foreach(F IN LISTS SOURCE_FILES)
	get_filename_component(EXENAME ${F} NAME_WE)
	add_executable(${EXENAME} ${F})
	target_include_directories(${EXENAME} PRIVATE ${SDL_INCLUDE_DIRS})
	target_link_libraries(${EXENAME} ${SDL_LIBRARIES})
endforeach ()
