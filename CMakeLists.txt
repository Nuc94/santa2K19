cmake_minimum_required(VERSION 3.0)

project(santa2K19)

#set(CMAKE_CXX_STANDARD 20)


############################################
#   MAIN EXECUTABLE
############################################
add_executable(main app/main.cpp)

############################################
#   PMHT LIBRARY
############################################
add_library(pmht
    include/santa2K19/pmht.h
    src/pmht/pmht.cpp
)

target_include_directories(pmht
    PUBLIC include
    PRIVATE src
)

target_link_libraries(main
    PUBLIC pmht
)

############################################
#   GTEST STUFF
############################################
set (gtest_force_shared_crt ON CACHE BOOL "MSVC defaults to shared CRT" FORCE)
add_subdirectory(third_party/googletest)
target_compile_definitions(gtest
    PUBLIC
        #GTEST_LANG_CXX20
        GTEST_HAS_TR1_TUPLE=0
)

############################################
#   TESTS
############################################
add_executable(tests tests/pmht_test.cpp)
#target_sources(tests
#    PRIVATE
#        tests/pmht_test.cpp
#)
#set_target_properties(tests PROPERTIES COMPILE_FLAGS "$cxx_strict")
target_link_libraries(tests gtest gtest_main pmht)