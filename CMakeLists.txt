cmake_minimum_required(VERSION 3.22)

project(Wordle VERSION 0.1)

add_executable(wordle main/main.cpp)

# Use the global version so all libraries can access it.
include_directories(include)

# Check the enviroment variable $OUTPUT to see what version we should build
# TODO: support multiple at once
# If the variable is set
if(DEFINED ENV{OUTPUT})
	# And the directory exists
	if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/output/$ENV{OUTPUT}  AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output/$ENV{OUTPUT})
		# Copy to a cmake variable
		set(OUTPUT $ENV{OUTPUT})
	# If it doesn't exist
	else()
		# Error and quit
		message(FATAL_ERROR "Output $ENV{OUTPUT} not supported.")
	endif()
# The variable is not set
else()
	# Default to ncurses output
	message(STATUS "No output specified, defaulting to ncurses.")
	set(OUTPUT "ncurses")
endif()

# Build the library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/output/${OUTPUT})

add_subdirectory(core)

# And link it
target_link_libraries(wordle wordle_${OUTPUT})
target_link_libraries(wordle wordle_core)

# Always run tests as they are still inexpensive
include(CTest)
add_subdirectory(tests)
