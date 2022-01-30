##################################################
# Define ez::cli::compile_options - a target
# representing a set of common compile options 
# for a project.
##################################################

include_guard()

add_library(ez_cli_compile_options INTERFACE)
add_library(ez::cli::compile_options ALIAS ez_cli_compile_options)

if(NOT EZ_CLI_STANDALONE)
    # use client defined options.
elseif(MSVC)
    target_compile_options(ez_cli_compile_options
        INTERFACE
        /EHsc
        /Wall
        /WX
        /Zc:__cplusplus
    )
else()
    target_compile_options(ez_cli_compile_options
        INTERFACE
        -Wall
        -Wcast-align
        -Wconversion
        -Werror
        -Wextra
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wpedantic
        -Wshadow
        -Wsign-conversion
        $<$<CXX_COMPILER_ID:GNU>:-fconcepts-diagnostics-depth=10>
    )
endif()

target_compile_features(ez_cli_compile_options INTERFACE cxx_std_20)

install(TARGETS ez_cli_compile_options EXPORT ez-libs)
set_target_properties(ez_cli_compile_options
    PROPERTIES
    EXPORT_NAME cli_compile_options)
