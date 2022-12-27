include_guard()

include(FetchContent)

FetchContent_Declare(ez-common
    GIT_REPOSITORY https://github.com/unixod/ez-common.git
    GIT_TAG v1.2.0
    GIT_SHALLOW On
)

FetchContent_Declare(ez-utils
    GIT_REPOSITORY https://github.com/unixod/ez-utils.git
    GIT_TAG v1.5.1
    GIT_SHALLOW On
)

# Enable installing of ez::utils if installating of ez::cli is enabled.
set(EZ_UTILS_INSTALL ${EZ_CLI_INSTALL})
FetchContent_MakeAvailable(ez-common ez-utils)

if(EZ_CLI_STANDALONE AND BUILD_TESTING)
    FetchContent_Declare(ez-catch
        GIT_REPOSITORY https://github.com/unixod/ez-catch.git
        GIT_TAG v3.2.1
        GIT_SHALLOW On
    )

    FetchContent_MakeAvailable(ez-catch)
endif()
