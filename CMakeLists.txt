cmake_minimum_required(VERSION 3.14)

project("Simple Calculator")

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(NO_GUI FALSE)
set(EXECUTABLE_NAME simple-calculator)

file(GLOB_RECURSE PROJECT_SOURCES "src/*")
if (NO_GUI OR NOT WIN32)
	add_executable(${EXECUTABLE_NAME} ${PROJECT_SOURCES})
	target_compile_definitions(${EXECUTABLE_NAME} PRIVATE NO_GUI)
else()
	add_executable(${EXECUTABLE_NAME} WIN32 ${PROJECT_SOURCES})
endif()
set_target_properties(${EXECUTABLE_NAME} PROPERTIES CXX_STANDARD 23)

add_subdirectory("deps/string-math")

target_include_directories(${EXECUTABLE_NAME} PRIVATE "deps/string-math/src")
target_link_libraries(${EXECUTABLE_NAME} PRIVATE "string-math")
