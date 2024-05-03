
# https://jonathanhamberg.com/post/cmake-embedding-git-hash/

set(pre_configure_file  ${RUKEN_SOURCES_DIR}/Src/Build/BuildInfo.cpp.in)
set(post_configure_file ${RUKEN_GENERATED_SOURCES_DIR}/Src/Build/BuildInfo.cpp)

function(GitWriteCache in_value in_cache_name)
    file(WRITE ${CMAKE_BINARY_DIR}/Git${in_cache_name}Cache.txt ${in_value})
endfunction()

function(GitReadCache out_value in_cache_name)
    if (EXISTS ${CMAKE_BINARY_DIR}/Git${in_cache_name}Cache.txt)
        file(STRINGS ${CMAKE_BINARY_DIR}/Git${in_cache_name}Cache.txt CONTENT)
        LIST(GET CONTENT 0 var)

        set(${out_value} ${var} PARENT_SCOPE)
    endif ()
endfunction()

function(CheckGitVersion)
    # Get the latest abbreviated commit hash of the working branch
    execute_process(
            COMMAND git log -1 --format=%h
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            OUTPUT_VARIABLE GIT_REVISION
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )

    execute_process(
            COMMAND git describe --abbrev=0 --tags
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            OUTPUT_VARIABLE GIT_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )

    if (GIT_REVISION STREQUAL "")
        set(GIT_REVISION "?")
    endif ()

    if (GIT_VERSION STREQUAL "")
        set(GIT_VERSION "?")
    endif ()

    GitReadCache(GIT_REVISION_CACHE Revision)
    GitReadCache(GIT_VERSION_CACHE Version)

    if (NOT DEFINED GIT_REVISION_CACHE)
        set(GIT_REVISION_CACHE "?")
    endif ()

    if (NOT DEFINED GIT_VERSION_CACHE)
        set(GIT_VERSION_CACHE "?")
    endif ()

    # Only update the git_version.cpp if the hash has changed. This will
    # prevent us from rebuilding the project more than we need to.
    if (NOT ${GIT_REVISION} STREQUAL ${GIT_REVISION_CACHE} OR
        NOT ${GIT_VERSION}  STREQUAL ${GIT_VERSION_CACHE}  OR
        NOT EXISTS ${post_configure_file})
        # Set che GIT_HASH_CACHE variable the next build won't have
        # to regenerate the source file.
        GitWriteCache(${GIT_REVISION} Revision)
        GitWriteCache(${GIT_VERSION} Version)

        configure_file(${pre_configure_file} ${post_configure_file})
    endif ()

endfunction()

function(CheckGitSetup)

    add_custom_target(AlwaysCheckGit COMMAND ${CMAKE_COMMAND}
            -DRUN_CHECK_GIT_VERSION=1
            -DRUKEN_SOURCES_DIR=${RUKEN_SOURCES_DIR}
            -DRUKEN_GENERATED_SOURCES_DIR=${RUKEN_GENERATED_SOURCES_DIR}
            -DGIT_HASH_CACHE=${GIT_HASH_CACHE}
            -P ${RUKEN_SOURCES_DIR}/CMake/CheckGit.cmake
            BYPRODUCTS ${post_configure_file}
    )

    add_library(git_version ${RUKEN_GENERATED_SOURCES_DIR}/Src/Build/BuildInfo.cpp)
    target_include_directories(git_version PUBLIC ${RUKEN_GENERATED_SOURCES_DIR}/Include)
    add_dependencies(git_version AlwaysCheckGit)

    CheckGitVersion()
endfunction()

# This is used to run this function from an external cmake process.
if (RUN_CHECK_GIT_VERSION)
    CheckGitVersion()
endif ()

