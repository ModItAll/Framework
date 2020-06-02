cmake_minimum_required(VERSION 3.15)
project(MIA-Framework C CXX)

set(MIA_VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
add_subdirectory(LCLib-CXX EXCLUDE_FROM_ALL)

include_directories(include LCLib-CXX/include)


find_package(BISON)
find_package(FLEX)

configure_file(FrameworkConfig.h.in FrameworkConfig.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(src)
add_library(mia-modules OBJECT src/ModuleFormat.hpp src/ModuleFormat.cpp)
set_target_properties(mia-modules PROPERTIES
        POSITION_INDEPENDENT_CODE ON)

target_link_libraries(mia-modules lc-c++)

add_executable(mia include/Framework.h src/Framework.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/FrameworkConfig.h)
target_link_libraries(mia mia-modules)

add_executable(modc src/ModCompiler.cpp)
target_link_libraries(modc mia-modules)
if(MSVC)
  target_compile_options(mia PRIVATE /W4 /WX)
else()
  target_compile_options(mia PRIVATE -Wall -pedantic -Werror)
endif()
configure_file(framework.mia.in framework.mia-src)
# add_custom_command(OUTPUT framework.mia
#        DEPENDS modc
#        MAIN_DEPENDENCY ${CMAKE_CURRENT_BINARY_DIR}/framework.mia-src
#        COMMAND $<TARGET_FILE:modc> -o ${CMAKE_CURRENT_BINARY_DIR}/framework.mia  ${CMAKE_CURRENT_BINARY_DIR}/framework.mia-src)
