# Define project information
cmake_minimum_required(VERSION 3.4.0)
project(PenguinProgrammerRPG)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Set sources
file(GLOB_RECURSE SOURCES "src/*.cpp")
include_directories(src/include)

# Set compiler flags
set(CMAKE_CXX_FLAGS "")
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "-std=c++11 -Weverything -Wno-comment -Wno-c++98-compat")
else()
	set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-comment")
endif()
set(PROJECT_LINK_LIBS m JsonBox)

# Add the final executable to be built
set(EXECUTABLE_NAME "pprpg")
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.3 COMPONENTS system graphics window)
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIRS})
	set(PROJECT_LINK_LIBS ${PROJECT_LINK_LIBS} ${SFML_LIBRARIES})
else()
	message(FATAL_ERROR "SFML not found, CMake will exit.")
endif()

# Link with libraries
target_link_libraries(${EXECUTABLE_NAME} ${PROJECT_LINK_LIBS})

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
