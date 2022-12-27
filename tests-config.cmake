include_guard()

###############################################################################
# Define ez::cli::tests_config
###############################################################################

add_library(ez_cli_tests_config INTERFACE)
add_library(ez::cli::tests_config ALIAS ez_cli_tests_config)

target_link_libraries(ez_cli_tests_config
    INTERFACE
    ez::catch
    ez::cli::private_compile_options
)

