# tests/CMakeLists.txt

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# model 'unit' files
file(GLOB_RECURSE unit ${CMAKE_SOURCE_DIR}/src/*.cpp)

set(TESTS tests) # replace "tests" here with the names of your test targets

foreach(TEST IN LISTS TESTS)
	add_executable(${TEST} ${CMAKE_SOURCE_DIR}/tests/${TEST}.cpp ${unit})
	target_include_directories(${TEST} PRIVATE ${CMAKE_SOURE_DIR})
	target_link_libraries(${TEST} PUBLIC Boost::unit_test_framework)
	set_target_properties(${TEST} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/tests/$<0:>)
endforeach()
    