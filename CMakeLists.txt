cmake_minimum_required(VERSION 3.24)

project(ez-cli)

include(GNUInstallDirs)
include(CMakeDependentOption)

##############################################################################
# Setup project general properties
##############################################################################

# Check if this is a standalone project or included by other project
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(EZ_CLI_STANDALONE On)
    include(CTest)
endif()

# If ez-cli is used by some outer project (e.g. via CMake FetchContent or add_subdirectory)
# then don't register targets defined here to be installed until it is explicitelly requested
# by the outer project by setting EZ_CLI_INSTALL option to On.
cmake_dependent_option(EZ_CLI_INSTALL
    "Generate an install target for ez::cli" Off "NOT EZ_CLI_STANDALONE" On)

# Require C++20 or higher.
# Notes:
# - CMAKE_CXX_STANDARD is for dependencies (for now it's only Catch2).
# - cxx_std_${EZ_CLI_CPP_MINIMUM_REQUIRED} (in compile-options.cmake) is for ez-support and for
#   its consumers (i.e. dependent targets).
set(EZ_CLI_CPP_MINIMUM_REQUIRED 20)
if(CMAKE_CXX_STANDARD VERSION_LESS ${EZ_CLI_CPP_MINIMUM_REQUIRED})
    set(CMAKE_CXX_STANDARD ${EZ_CLI_CPP_MINIMUM_REQUIRED})
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(dependencies.cmake)
include(compile-options.cmake)
include(custom-build-profiles.cmake)

##############################################################################
# Setup ez::cli target
##############################################################################

add_library(ez_cli INTERFACE)
add_library(ez::cli ALIAS ez_cli)

target_include_directories(ez_cli
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(ez_cli
    INTERFACE
    ez::cli::public_compile_options
    ez::utils)

if(EZ_CLI_INSTALL)
    install(DIRECTORY include/ez TYPE INCLUDE)

    # Note: an export 'ez-libs' is defined at ez-support.
    install(TARGETS ez_cli EXPORT ez-libs)

    set_target_properties(ez_cli
        PROPERTIES
        EXPORT_NAME cli
    )
endif()

##############################################################################
# Setup unit tests
##############################################################################
if (EZ_CLI_STANDALONE AND BUILD_TESTING)
    include(tests-config.cmake)
    add_subdirectory(tests)
endif()

