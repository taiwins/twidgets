cmake_minimum_required(VERSION 3.12.0)
project(twidgets
  VERSION 0.1
  LANGUAGES C)

add_definitions(-D_GNU_SOURCE)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

###############################################################################
# options
###############################################################################
option(NKCAIRO "Enabling twidgets-cairo backend" ON)
option(NKEGL   "Enabling twidgets-egl backend" ON)
option(NKLUA   "Enabling lua bindings" ON)
option(TESTS   "Enabling tests" ON)

###############################################################################
# compiler flags
###############################################################################

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_definitions(-D__DEBUG)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb3 -Wall -Wextra -fsanitize=address")
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -fvisibility=hidden")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wno-unused-parameter -Wno-pedantic")

###############################################################################
# find packages
###############################################################################

find_package(PkgConfig REQUIRED)
find_package(Pixman REQUIRED)
find_package(XKBCommon REQUIRED)
find_package(Wayland REQUIRED)
find_package(WaylandProtocols REQUIRED)
find_package(Cairo REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS EGL OpenGL)
find_package(NVEGL)
find_package(Fontconfig REQUIRED)
find_package(Freetype REQUIRED)
find_package(UDEV REQUIRED)
find_package(LibRSVG REQUIRED)
find_package(twclient 0.2.1 REQUIRED)

###############################################################################
# subdirs
###############################################################################
set(TWIDGETS_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

###############################################################################
# protocols
###############################################################################
include(AddProtocols)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)
