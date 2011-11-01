PROJECT(ninja)
cmake_minimum_required(VERSION 2.8)

find_package(GTest)

#NB: We cannot use GLOB here..(Using GLOB kills auto-regeneration)

set(srcs_main
    src/ninja.cc)
set(srcs_base
src/build.cc
src/build_log.cc
src/clean.cc
src/disk_interface.cc
src/eval_env.cc
src/graph.cc
src/graphviz.cc
src/stat_cache.cc
src/parsers.cc
src/state.cc
src/util.cc
    )

set(srcs_posix
src/browse.cc
src/subprocess.cc
    )

set(test_srcs
src/clean_test.cc
src/build_log_test.cc
src/build_test.cc
src/disk_interface_test.cc
src/eval_env_test.cc
src/graph_test.cc
src/parser_perftest.cc
src/parsers_test.cc
src/state_test.cc
src/subprocess_test.cc
src/util_test.cc
)

set(srcs_win32
src/subprocess-win32.cc
)

set(test_lib_srcs
    src/test.cc)

if(WIN32)
    set(srcs ${srcs_base} ${srcs_win32})
else()
    set(srcs ${srcs_base} ${srcs_posix})
endif()

add_library(ninjabody STATIC ${srcs})
add_executable(ninja ${srcs_main})
target_link_libraries(ninja ninjabody)

if(CMAKE_COMPILER_IS_GNUCXX)
    if(WIN32)
        add_definitions(-DWINVER=0x501) # GetProcessId
    endif()
    set_target_properties(ninja ninjabody
        PROPERTIES
        COMPILE_FLAGS "-fno-exceptions -fvisibility=hidden -Wno-deprecated -fpermissive")
endif()

if(GTEST_INCLUDE_DIRS)
    enable_testing()
    add_library(ninjatest STATIC ${test_lib_srcs})
    include_directories(${GTEST_INCLUDE_DIRS})
    foreach(t ${test_srcs})
        get_filename_component(tname ${t} NAME_WE)
        add_executable(${tname} ${t})
        add_test(${tname} ${tname})
        target_link_libraries(${tname}
            ninjatest
            ninjabody
            ${GTEST_BOTH_LIBRARIES})

    endforeach()
endif()
