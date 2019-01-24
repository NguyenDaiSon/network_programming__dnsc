# Nguyen Dai Son

cmake_minimum_required (VERSION 2.8)

include_directories(src)

file(GLOB_RECURSE dnsc__src_files src/mbuf.cpp src/mbuf.hpp src/dnsc.cpp src/dnsc.hpp)

if(WIN32 OR CYGWIN OR MINGW)
	set(IS_WINDOWS TRUE)
else()
	set(IS_WINDOWS FALSE)
endif()

add_executable(dnsc src/main.cpp ${dnsc__src_files})
if (IS_WINDOWS)	
	target_link_libraries(dnsc ws2_32)
endif()
