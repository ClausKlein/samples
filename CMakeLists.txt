cmake_minimum_required(VERSION 3.13)

## use ccache if found
find_program(CCACHE_EXECUTABLE ccache HINTS /usr/local/bin /opt/local/bin)
if(CCACHE_EXECUTABLE AND NOT CMAKE_TOOLCHAIN_FILE)
    message(STATUS "use ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_EXECUTABLE}" CACHE PATH "ccache" FORCE)
endif()


#=============================
project(samples LANGUAGES CXX)
#=============================

set(CMAKE_MODULE_PATH $ENV{HOME}/cmake)

#---------------------------------------------------------------------------------------
# Compiler config
#---------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(COMPILER_WARNINGS_ARE_ERRORS "To be pedantic! ;-)" ON)
if(COMPILER_WARNINGS_ARE_ERRORS)
  if(MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
  else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -Wpedantic -Werror
      -Wno-unused-parameter
      -Wno-unused-variable
      -Wno-unused-result
      -Wno-unknown-warning-option
    )
  endif()
endif()

option(USE_CPPCHECK "run cppcheck along with the compiler and report any problems" OFF)
option(USE_CLANG_TIDY "run clang-tidy along with the compiler and report any problems" OFF)

set(CMAKE_DEPFILE_FLAGS_CXX "-MMD" CACHE STRING "dependency flag" FORCE)
#TODO set(CMAKE_DEPFILE_FLAGS_CXX "-MMD -MT <OBJECT> -MF <DEPFILE>" CACHE STRING "dependency flag" FORCE)
option(CMAKE_CXX_DEPFILE_EXTENSION_REPLACE "name depend files as main.d instead of main.cpp.d" ON)
option(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE "name object files as main.o instead of main.cpp.o" ON)
option(CMAKE_DEPENDS_IN_PROJECT_ONLY "do not use system header files" ON)


if(USE_CLANG_TIDY)
    find_program(CMAKE_CXX_CLANG_TIDY clang-tidy HINTS $ENV{HOME}/bin /usr/local/bin /opt/local/bin)
elseif(USE_CPPCHECK)
    find_program(CMAKE_CXX_CPPCHECK cppcheck HINTS /usr/local/bin /opt/local/bin)
endif()


if(UNIX)
    option(USE_LCOV "Use GNU Test Coverage framework" OFF)
    find_library(PTHREAD_LIB pthread REQUIRED)
endif()

# see /usr/local/lib/cmake/gsl-lite/gsl-lite-targets.cmake
find_package(gsl-lite 0.3.7 REQUIRED)
#XXX find_package(boost_headers REQUIRED)
find_package(Boost 1.67 REQUIRED COMPONENTS system)
find_package(doctest 2.3.4 REQUIRED)
#XXX find_package(Microsoft.GSL REQUIRED)

# Where to put all the RUNTIME targets when built.  This variable is used to
# initialize the RUNTIME_OUTPUT_DIRECTORY property on all the targets.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

enable_testing()

set(cxx_sources
    access_privates.cpp
    at_test.cpp
    bitset.cpp
    byteorder.cpp
    bounds_test.cpp
    chainOfResponsibility.cpp
    ClonableBase.cpp
    classOutput.cpp
    callback.cpp
    clone.cpp
    NonCopyable.cpp
    DoNotSlice.cpp
    CloneShapes.cpp
    #!NO! collate.cpp
    command-pattern.cpp
    #XXX copy.cpp
    delegate-pattern.cpp
    dependency-injection.cpp
    #!NO! endlessloop.cpp
    EreaseRemoveIdiom.cpp
    example-style.cpp
    #!NO! exceptions.cpp
    factory_function.cpp
    friend-class.cpp
    #!NO! getaddrinfo.cpp
    #!NO! getline.cpp
    hash.cpp
    ideone_nWFr9w.cpp
    immutableString.cpp
    InnerClassIdiom.cpp
    konto.cpp
    l1083.cpp
    map.cpp
    nested-classes.cpp
    NonVirtualBaseClassTest.cpp
    permissive.cpp
    point.cpp
    Pimple.cpp
    #XXX no main() User.cpp
    #XXX no main() PimplePattern.cpp
    PostInitialize.cpp
    priority_queue.cpp
    Pythagorean_triple.cpp
    result_of.cpp
    Round.cpp
    set2.cpp
    sets.cpp
    slide.cpp
    SFINAE-cxx11.cpp
    sorted_map.cpp
    sorted_set.cpp
    stripws.cpp
    stream_operators.cpp
    test_filbuf.cpp
    #TODO needs data.txt test_gcount.cpp
    test_iomanip.cpp
    test_rdbuf.cpp
    testclock.cpp
    #TODO needs data.txt testiostream.cpp
    timevalue.cpp
    trim_example.cpp
    unordered_map.cpp
    unordered_set.cpp
    u8string.cpp
    use_span.cpp
    vector.cpp
    vokable2.cpp
    #FIXME volatile.cpp
)

foreach(source ${cxx_sources})
    string(REGEX REPLACE "\.cpp$" "" program ${source})
    add_executable(${program} ${program}.cpp)
    target_include_directories(${program} PRIVATE SYSTEM ${Boost_INCLUDE_DIRS} .)
    #FIXME: only supported from gsl-lite! gsl_CONFIG_CONTRACT_VIOLATION_THROWS
    target_compile_definitions(${program} PRIVATE
        -Dgsl_CONFIG_CONTRACT_VIOLATION_THROWS
    )
    target_link_libraries(${program}
        PRIVATE doctest::doctest gsl::gsl-lite-v1 ${Boost_LIBRARIES} ${PTHREAD_LIB})
    set_target_properties(${program} PROPERTIES CXX_STANDARD 17)
    add_test(NAME ${program} COMMAND ${program})
endforeach()


list(APPEND CMAKE_MODULE_PATH $ENV{HOME}/Workspace/cmake)
if(USE_LCOV)
    set(CMAKE_BUILD_TYPE "Coverage" CACHE STRING "build for lcov" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    include(CodeCoverage)
    setup_target_for_coverage(
        lcov            # Name for custom target.
        ctest           # Name of the test driver executable that runs the tests.
                        # NOTE! This should always have a ZERO as exit code
                        # otherwise the coverage generation will not complete.
        coverage        # Name of output directory.
    )
endif() #CMAKE_BUILD_TYPE STREQUAL "Coverage"

include(CMakePrintSystemInformation)
message("CMAKE_CXX_LIBRARY_ARCHITECTURE ${CMAKE_CXX_LIBRARY_ARCHITECTURE}")

