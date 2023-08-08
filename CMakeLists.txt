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

if(${PROJECT_NAME}_ENABLE_TESTING)
    if(NOT TARGET Catch)
        cpmaddpackage("gh:catchorg/Catch2@3.3.2")
    endif()
endif()

# ---- Create binary ----

add_library(${PROJECT_NAME} src/debounced_button.c)
target_include_directories(${PROJECT_NAME} PUBLIC src)
# target_link_libraries(${PROJECT_NAME} PUBLIC c_static_asserts)

if(${PROJECT_NAME}_ENABLE_TESTING)
    add_executable(${PROJECT_NAME}_tests test/test_debounced_button.cpp)
    target_link_libraries(
        ${PROJECT_NAME}_tests Catch2::Catch2WithMain ${PROJECT_NAME}
    )
    target_compile_features(${PROJECT_NAME}_tests PRIVATE cxx_std_17)
    target_compile_options(
        ${PROJECT_NAME}_tests
        PRIVATE "-g"
                "-gdwarf-4"
                "-Wno-c99-designator"
                "-Wno-extern-c-compat"
    )

    # ---- Enable testing ----

    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
    include(CTest)
    include(Catch)
    catch_discover_tests(${PROJECT_NAME}_tests)
endif()
