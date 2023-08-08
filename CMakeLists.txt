cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(debounced_button)

# ---= Build Configurations ----
if(NOT DEFINED ${PROJECT_NAME}_ENABLE_TESTING)
    set(${PROJECT_NAME}_ENABLE_TESTING
        ${debounced_button_IS_TOP_LEVEL}
        CACHE BOOL "Build tests for ${PROJECT_NAME}"
    )
endif()

# ---- Dependencies ----

include(cmake/CPM.cmake)

# if(${PROJECT_NAME}_ENABLE_TESTING)
#     if(NOT TARGET Catch)
#         cpmaddpackage("gh:catchorg/Catch2@3.3.2")
#     endif()
# endif()

# if(NOT TARGET c_static_asserts)
#     cpmaddpackage("gh:drbartling/c_static_asserts#master")
# endif()

# ---- Create binary ----

add_library(${PROJECT_NAME} src/debounced_button.c)
target_include_directories(${PROJECT_NAME} PUBLIC src)
# target_link_libraries(${PROJECT_NAME} PUBLIC c_static_asserts)

if(${PROJECT_NAME}_ENABLE_TESTING)
    add_executable(
        ${PROJECT_NAME}_tests test/test_debounced_button.c unity/unity.c
    )
    target_include_directories(${PROJECT_NAME}_tests PRIVATE unity test)
    target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME})

    include(CTest)
    add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)
endif()
