include_guard()

if(FEATURE_TESTS)
    if(NOT TARGET Catch)
        cpmaddpackage("gh:catchorg/Catch2@3.7.1")
    endif()
endif()
