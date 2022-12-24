include_guard()

include(FetchContent)

###########################################
# Fetch dependencies
###########################################
FetchContent_Declare(ez-catch
    GIT_REPOSITORY https://github.com/unixod/ez-catch.git
    GIT_TAG 7e58663ab2ace81cd839c92f4ba804606847403e
    GIT_SHALLOW On
)

FetchContent_MakeAvailable(ez-catch)

###########################################
# Add ez::tests_config
###########################################
add_library(ez_cli_tests_config INTERFACE)
add_library(ez::cli::tests_config ALIAS ez_cli_tests_config)

target_link_libraries(ez_cli_tests_config
    INTERFACE
    ez::catch)

#target_compile_options(ez_cli_tests_config
#    INTERFACE
#    -fno-omit-frame-pointer
#    -fsanitize=address
#    -fsanitize=undefined)
