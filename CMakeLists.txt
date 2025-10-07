cmake_minimum_required(VERSION 3.15...4.1 FATAL_ERROR)

# https://cliutils.gitlab.io/modern-cmake/chapters/intro/installing.html
# PIPx might be one of the easiest ways to keep cmake up to date
# https://pypi.org/project/cmake/

project(
    debounced_button
    DESCRIPTION "A simple debounce filter for button input"
    LANGUAGES CXX C
)

include(cmake/CPM.cmake)
include(cmake/dependencies.cmake)

# ---- Create binary ----

add_library(${PROJECT_NAME} src/debounced_button.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

if(FEATURE_TESTS)
    add_executable(
        ${PROJECT_NAME}_tests test/main.cpp test/test_debounced_button.cpp
    )
    target_link_libraries(${PROJECT_NAME}_tests Catch2::Catch2 ${PROJECT_NAME})

    # ---- Windows-specific configuration ----
    if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # Set console application properties
        set_target_properties(
            ${PROJECT_NAME}_tests PROPERTIES WIN32_EXECUTABLE FALSE
        )
        # Add static linking to reduce DLL dependencies
        target_link_options(
            ${PROJECT_NAME}_tests
            PRIVATE
            -static-libgcc
            -static-libstdc++
        )
    endif()

    # ---- Enable testing ----

    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
    include(CTest)
    include(Catch)

    # Windows GCC has runtime issues with test discovery, use simple test registration
    if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)
    else()
        catch_discover_tests(${PROJECT_NAME}_tests)
    endif()
endif()
