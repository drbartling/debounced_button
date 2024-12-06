cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(debounced_button LANGUAGES CXX C)

# ---- Dependencies ----

include(cmake/CPM.cmake)
include(cmake/dependencies.cmake)
include(cmake/project_options.cmake)

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

    # ---- Enable testing ----

    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
    include(CTest)
    include(Catch)
    catch_discover_tests(${PROJECT_NAME}_tests)
endif()
