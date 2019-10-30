cmake_minimum_required(VERSION 3.9)

project(lexer_test1)

file( GLOB_RECURSE  SOURCE_INC "../*.h")
file( GLOB_RECURSE  SOURCE_SRC "../test1.cpp" )

source_group("inc" FILES ${SOURCE_INC} )
source_group("src" FILES ${SOURCE_SRC} )

add_executable(lexer_test1 ${SOURCE_SRC} ${SOURCE_INC} )

target_compile_definitions(lexer_test1
    PRIVATE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        UNICODE
        _UNICODE
        _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
        _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
        )


set_target_properties( lexer_test1 PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

project(lexer_test2)

file( GLOB_RECURSE  SOURCE_INC "../*.h")
file( GLOB_RECURSE  SOURCE_SRC "../test2.cpp" )

source_group("inc" FILES ${SOURCE_INC} )
source_group("src" FILES ${SOURCE_SRC} )

add_executable(lexer_test2 ${SOURCE_SRC} ${SOURCE_INC} )

target_compile_definitions(lexer_test2
    PRIVATE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        UNICODE
        _UNICODE
        _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
        _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
        )


set_target_properties( lexer_test2 PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

project(lexer_test3)

file( GLOB_RECURSE  SOURCE_INC "../*.h")
file( GLOB_RECURSE  SOURCE_SRC "../test3.cpp" )

source_group("inc" FILES ${SOURCE_INC} )
source_group("src" FILES ${SOURCE_SRC} )

add_executable(lexer_test3 ${SOURCE_SRC} ${SOURCE_INC} )

target_compile_definitions(lexer_test3
    PRIVATE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        UNICODE
        _UNICODE
        _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
        _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
        )


set_target_properties( lexer_test3 PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

