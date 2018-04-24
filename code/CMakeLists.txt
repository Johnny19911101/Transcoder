cmake_minimum_required (VERSION 2.6)
message(${FFMPEG})
#set(CMAKE_CXX_FLAGS "-pedantic-errors -DTIMER")
project (Transcode)
if(UNIX)
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 ")
	find_package (Threads)
	add_definitions("-DLINUX -O2 -s -Wall -Dlinux")

	if(COMPILER_SUPPORTS_CXX11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	elseif(COMPILER_SUPPORTS_CXX0X)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else()
		message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no
			C++11 support. Please use a different C++ compiler.")
	endif()
endif(UNIX)
file(GLOB SOURCES 
	src/Audio.cpp
	src/Timer.cpp
	src/CtxWrapper.cpp
	src/Option.cpp
	src/TransState.cpp
	src/Transcoder.cpp
	src/Video.cpp
	src/Adinsert.cpp
)
include_directories(${CMAKE_SOURCE_DIR}/media_core/code/inc)
#link_directories(${CMAKE_SOURCE_DIR}/../lib)
add_executable(Transcode ${SOURCES} src/main.cpp)
add_library(NoovoTranscode SHARED ${SOURCES})
#set(ffmpeg_lib avformat avfilter avcodec swresample swscale avutil avdevice x264 fdk-aac postproc)
set(UnixLib z m pthread dl)
target_link_libraries(Transcode ${FFMPEG_LIB})
target_link_libraries(NoovoTranscode ${FFMPEG_LIB})
target_link_libraries(NoovoTranscode ${LOGGER_LIB})
if(UNIX)
target_link_libraries(Transcode ${UnixLib})
endif(UNIX)
