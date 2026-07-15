cmake_minimum_required( VERSION 3.50 )

project( fps VERSION 0.1 LANGUAGES CXX)
set(PROJECT_DESCRIPTION "C++ FPS timer library")

if(NOT CMAKE_GENERATOR MATCHES "^Visual Studio")
include(GNUInstallDirs)
include(FindPkgConfig)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE )
endif()

string(TOUPPER "${CMAKE_SYSTEM_PROCESSOR}" SYSTEM_PROCESSOR_UPPER)
message(STATUS "OS: ${CMAKE_HOST_SYSTEM_NAME}-${CMAKE_HOST_SYSTEM_PROCESSOR} => ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CC: ${CMAKE_CXX_COMPILER_ID}/${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

add_compile_definitions(_FILE_OFFSET_BITS=64)
add_compile_definitions($<$<CONFIG:Debug>:DEBUG=1> $<$<NOT:$<CONFIG:Debug>>:NDEBUG=1>)
add_compile_definitions($<$<CONFIG:Debug>:_DEBUG=1> $<$<NOT:$<CONFIG:Debug>>:_NDEBUG=1>)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wno-gnu-alignof-expression)
endif()

if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "GNU|AppleClang")
	if( NOT CMAKE_BUILD_OPTLEVEL )
		set( CMAKE_BUILD_OPTLEVEL 3 CACHE STRING "Choose the optimization level." FORCE )
	endif()
	add_compile_options(-O${CMAKE_BUILD_OPTLEVEL})
	if (NOT CMAKE_CROSSCOMPILING)
		add_compile_options(-march=native)

		if (${SYSTEM_PROCESSOR_UPPER} MATCHES "X64|X86_64|AMD64")
			add_compile_options(-mfpmath=sse)
		endif()
		if (${SYSTEM_PROCESSOR_UPPER} MATCHES "PPC|POWERPC|PPC64|PPC64LE")
			include(CheckCSourceCompiles)

			set(CMAKE_REQUIRED_FLAGS "-maltivec")
			check_cxx_source_compiles("#include <altivec.h> int main() { vector int v = (vector int){0}; return 0; }" HAVE_ALTIVEC)

			if(HAVE_ALTIVEC)
				add_compile_options(-maltivec)
			endif()
		endif()
	endif()
	add_compile_options(-ftree-vectorize)
	add_compile_options(-fopenmp)
	if(NOT CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "AppleClang")
		add_compile_options(-fopenmp-simd)
	endif()
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		add_compile_options(-g)
	else()
		# Use IPO/LTO if available
		if(NOT ${CMAKE_VERSION} VERSION_LESS "3.9.0")
			cmake_policy(SET CMP0069 NEW)
			include(CheckIPOSupported)
			check_ipo_supported(RESULT result)
			if(result)
				set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
				message(STATUS "Using IPO/LTO.")
			endif()
		endif()
	endif()
	add_compile_options(-Wall)
	add_compile_options(-Wextra)
	add_compile_options(-Wno-write-strings)
	add_compile_options(-Wno-unused-result)
	add_compile_options(-Wno-unused-variable)
	add_compile_options(-Wno-unused-but-set-variable)
	add_compile_options(-Wno-unused-parameter)
	add_compile_options(-Wno-missing-field-initializers)
	add_compile_options(-Wunreachable-code)		# Warn about unreachable code paths
	add_compile_options(-fno-asynchronous-unwind-tables)
	add_compile_options(-ffunction-sections)	# Put each function in its own section
	add_compile_options(-fdata-sections)		# Put each variable in its own section
	if (CMAKE_BUILD_TYPE MATCHES ReleaseStrip)
		add_link_options(-Wl,--gc-sections)
		add_link_options(-Wl,-s)
	endif()
	link_libraries(m)
	if (WIN32 AND MINGW)
		link_libraries(mingw32)
	endif()
else (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
	string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
	string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	add_compile_options(/openmp:experimental)
	add_compile_options(/Zc:__cplusplus)
	add_compile_options(/std:c++latest)
	add_compile_options(/Zc:preprocessor)
	add_compile_options(/Z7)
	add_compile_options(/Gy)					# Function-level linking (puts each function in its own section)
	add_compile_options(/Gw)					# Optimize global data (puts each variable in its own section)
	if (NOT CMAKE_CROSSCOMPILING)
		add_compile_definitions(_CRT_SECURE_NO_WARNINGS _WIN32)
		if (CMAKE_VS_PLATFORM_NAME MATCHES "x64|Win32")
			#add_compile_options(/Gv)
		endif()
	endif()
	if (CMAKE_BUILD_TYPE MATCHES ReleaseStrip)
		add_link_options(/DEBUG:NONE)
		add_link_options(/OPT:REF)				# Eliminate unreferenced functions and data
		add_link_options(/VERBOSE:REF)			# Print out everything the linker throws away
	endif()
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		add_compile_options(/Zi)
		add_link_options(/DEBUG:FULL)
	else()
		if( NOT CMAKE_BUILD_OPTLEVEL )
			set( CMAKE_BUILD_OPTLEVEL 2 CACHE STRING "Choose the optimization level." FORCE )
		endif()
		add_compile_options(/O${CMAKE_BUILD_OPTLEVEL})
	endif()
	add_link_options(/INCREMENTAL:NO)
endif()

include_directories( ${PROJECT_NAME} INTERFACE
        PUBLIC_HEADER $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_executable( ttyfps ${PROJECT_SOURCE_DIR}/src/ttyfps.cpp )
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories( ttyfps INTERFACE
        PUBLIC_HEADER $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/sys DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT headers)

