include_guard()

cpmusepackagelock(package-lock.cmake)

if(FEATURE_TESTS)
    if(NOT TARGET Catch)
        cpmaddpackage("gh:catchorg/Catch2@3.11.0")
    endif()
endif()
