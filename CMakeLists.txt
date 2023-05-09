cmake_minimum_required(VERSION 3.12)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(v8pp
	VERSION 2.1.0
	DESCRIPTION "Bind C++ functions and classes into V8 JavaScript engine"
	HOMEPAGE_URL https://github.com/pmed/v8pp
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_TESTING "Build and run tests" ON)
option(BUILD_DOCUMENTATION "Build documentation" OFF)
set(V8_ROOT_DIR ${PROJECT_SOURCE_DIR}/../v8-cmake)
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install" CACHE PATH "install here" FORCE)

# Custom project options
set(V8PP_HEADER_ONLY 0 CACHE BOOL "Header-only library")
set(V8PP_ISOLATE_DATA_SLOT 0 CACHE STRING "v8::Isolate data slot number, used in v8pp for shared data")
set(V8PP_PLUGIN_INIT_PROC_NAME "v8pp_module_init" CACHE STRING "v8pp plugin initialization procedure name")
set(V8PP_PLUGIN_SUFFIX ${CMAKE_SHARED_MODULE_SUFFIX} CACHE STRING "v8pp plugin filename suffix")
set(V8_COMPRESS_POINTERS 0 CACHE BOOL "Use new V8 ABI with V8_COMPRESS_POINTERS and V8_31BIT_SMIS_ON_64BIT_ARCH")

if(BUILD_SHARED_LIBS AND WIN32)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)
endif()

# put all binaries into the output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

set(disable-exceptions
  $<$<CXX_COMPILER_ID:MSVC>:/EHs-c->
  $<$<CXX_COMPILER_ID:AppleClang>:-fno-exceptions>
  $<$<CXX_COMPILER_ID:Clang>:-fno-exceptions>
  $<$<CXX_COMPILER_ID:GNU>:-fno-exceptions>
)

set(enable-exceptions
  $<$<CXX_COMPILER_ID:MSVC>:/EHsc>
  $<$<CXX_COMPILER_ID:AppleClang>:-fexceptions>
  $<$<CXX_COMPILER_ID:Clang>:-fexceptions>
  $<$<CXX_COMPILER_ID:GNU>:-fexceptions>
)


add_subdirectory(v8pp)

if(BUILD_TESTING)
	enable_testing()
	add_subdirectory(plugins)
	add_subdirectory(test)
endif()

if(BUILD_DOCUMENTATION)
	add_subdirectory(docs)
endif()
