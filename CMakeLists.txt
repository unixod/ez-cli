cmake_minimum_required(VERSION 3.16)

project(ez-cli
    VERSION 1.0.0
    LANGUAGES CXX
)

include(GNUInstallDirs)

# FIXME: Remove the following pair of lines before commit
set(CMAKE_C_FLAGS "-fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
set(CMAKE_CXX_FLAGS "-fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")

##############################################################################
# Setup project general properties
##############################################################################

# Check if this is a standalone project or included by other project
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(EZ_CLI_STANDALONE On)
endif()

# Although this library is supposed to be dependency free its unit tests
# are based on Catch2 unit test framework. Specifying CMAKE_CXX_STANDARD
# in the following block is necessary for correct building of and linking
# with Catch2 and potentialy any other fetched dependency because they
# need to be informed somehow about the C++ standard being used.
if(EZ_CLI_STANDALONE AND NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

##############################################################################
# Fetch dependencies
##############################################################################
include(FetchContent)

FetchContent_Declare(ez-utils
    GIT_REPOSITORY https://github.com/unixod/ez-utils.git
#    GIT_TAG v1.2.0
    GIT_TAG 9b3918061a914e5aa40198fa7863367b6b8ff9cf
    GIT_SHALLOW On)

FetchContent_MakeAvailable(ez-utils)

##############################################################################
# Setup ez::cli target
##############################################################################
include(compile-options.cmake)

add_library(ez_cli INTERFACE)
add_library(ez::cli ALIAS ez_cli)

target_include_directories(ez_cli
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(ez_cli
    INTERFACE
    ez::cli::compile_options
    ez::utils)

install(DIRECTORY include/ez TYPE INCLUDE)
install(TARGETS ez_cli EXPORT ez-libs)
set_target_properties(ez_cli
    PROPERTIES
    EXPORT_NAME cli)


##############################################################################
# Setup unit tests
##############################################################################
if (EZ_CLI_STANDALONE)
    include(CTest)

    if (BUILD_TESTING)
        include(tests-config.cmake)
        add_subdirectory(tests)
    endif()
endif()


