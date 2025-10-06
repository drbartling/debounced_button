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
include(cmake/project_options.cmake)
include(cmake/dependencies.cmake)

# ---- Create binary ----

add_library(${PROJECT_NAME} src/debounced_button.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE project_options project_warnings)

if(FEATURE_TESTS)
    add_executable(${PROJECT_NAME}_tests test/test_debounced_button.cpp)
    target_link_libraries(
        ${PROJECT_NAME}_tests Catch2::Catch2WithMain ${PROJECT_NAME}
    )

    # ---- Windows-specific console application configuration ----
    if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set_target_properties(
            ${PROJECT_NAME}_tests PROPERTIES WIN32_EXECUTABLE FALSE
        )
        target_link_options(
            ${PROJECT_NAME}_tests
            PRIVATE
            -Wl,--subsystem,console
            -Wl,-e,mainCRTStartup
        )
    endif()

    # ---- Enable testing ----

    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
    include(CTest)
    include(Catch)
    catch_discover_tests(${PROJECT_NAME}_tests)
endif()
