cmake_minimum_required(VERSION 3.13)

## use ccache if found
find_program(CCACHE_EXECUTABLE "ccache" HINTS /usr/local/bin /opt/local/bin)
if(CCACHE_EXECUTABLE AND NOT CMAKE_TOOLCHAIN_FILE)
    message(STATUS "use ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_EXECUTABLE}" CACHE PATH "ccache" FORCE)
endif()


project(samples LANGUAGES C CXX)

#---------------------------------------------------------------------------------------
# Compiler config
#---------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS -Wpedantic)

enable_testing()

set(c_sources
    #XXX copy_stdio.c
    #TODO misra-test.c
    teststdio.c
)

set(cxx_sources
    bitset.cpp
    byteorder.cpp
    chainOfResponsibility.cpp
    ClonableBase.cpp
    classOutput.cpp
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
    PostInitialize.cpp
    priority_queue.cpp
    Pythagorean_triple.cpp
    result_of.cpp
    set2.cpp
    sets.cpp
    SFINAE-cxx11.cpp
    sorted_map.cpp
    sorted_set.cpp
    stripws.cpp
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
    use_span.cpp
    vector.cpp
    vokable2.cpp
    volatile.cpp
)

foreach(source ${cxx_sources})
    string(REGEX REPLACE "\.cpp$" "" program ${source})
    add_executable(${program} ${program}.cpp)
    set_target_properties(${program} PROPERTIES CXX_STANDARD 17)
    # target_link_libraries(${program} ${EXTRA_LIBS})
    add_test(NAME ${program} COMMAND ${program})
endforeach()

foreach(source ${c_sources})
    string(REGEX REPLACE "\.c$" "" program ${source})
    add_executable(${program} ${program}.c)
    add_test(NAME ${program} COMMAND ${program})
endforeach()

