include_guard()

cpmaddpackage("gh:aminya/project_options@0.41.0")

option(FEATURE_TESTS "Enable the tests" OFF)
option(FEATURE_DOCS "Enable the docs" OFF)

if(FEATURE_TESTS)
    set(ENABLE_COVERAGE "ENABLE_COVERAGE")
endif()

if(FEATURE_DOCS)
    set(ENABLE_DOXYGEN "ENABLE_DOXYGEN")
endif()

project_options(
    ENABLE_CACHE
    ENABLE_VS_ANALYSIS
    ${ENABLE_DOXYGEN}
    ${ENABLE_COVERAGE}
)
