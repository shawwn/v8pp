# test target

add_executable(v8pp_test
	main.cpp
	test.hpp
	test_call_from_v8.cpp
	test_call_v8.cpp
	test_class.cpp
	test_context.cpp
	test_convert.cpp
	test_function.cpp
	test_json.cpp
	test_module.cpp
	test_object.cpp
	test_property.cpp
	test_ptr_traits.cpp
	test_throw_ex.cpp
	test_utility.cpp
)

if(V8PP_HEADER_ONLY)
	target_sources(v8pp_test PRIVATE ${PROJECT_SOURCE_DIR}/v8pp/context.cpp)
	if(APPLE)
		# see v8pp/CMakeLists.txt
		set_source_files_properties(${PROJECT_SOURCE_DIR}/v8pp/context.cpp PROPERTIES COMPILE_FLAGS -fno-rtti)
	endif()
endif()

#set(CMAKE_CXX_RTTI OFF)
if(MSVC)
	set(V8PP_COMPILE_OPTIONS /GR- /EHsc /permissive- /W4)
	# disable specific warnings
	list(APPEND V8PP_COMPILE_OPTIONS /wd4190)
	# set warning level 3 for system headers
	list(APPEND V8PP_COMPILE_OPTIONS /experimental:external /external:anglebrackets /external:W3)
else()
	set(V8PP_COMPILE_OPTIONS -frtti -fexceptions -Wall -Wextra -Wpedantic)
endif()
#set(V8PP_LINK_OPTIONS --eh-frame-hdr)

target_link_libraries(v8pp_test v8pp)# ${CMAKE_DL_LIBS})
target_compile_options(v8pp_test PUBLIC ${V8PP_COMPILE_OPTIONS})
target_link_options(v8pp_test PUBLIC ${V8PP_LINK_OPTIONS})
message(STATUS "FOO: ${V8PP_COMPILE_OPTIONS}")
message(STATUS "FOO: ${V8PP_LINK_OPTIONS}")

add_test(v8pp_test ${PROJECT_BINARY_DIR}/v8pp_test --version --run-tests)

if(BUILD_SHARED_LIBS)
	file(GLOB JS_TESTS *.js)
	add_test(v8pp_js_test ${PROJECT_BINARY_DIR}/v8pp_test --lib-path ${PROJECT_BINARY_DIR}/plugins ${JS_TESTS})
endif()
