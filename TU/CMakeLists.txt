# ------------------------------
# Téléchargement automatique de GoogleTest
# ------------------------------
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Empêche gtest d'essayer de compiler ses propres tests
set(gtest_disable_pthreads OFF)
set(BUILD_GMOCK ON)
set(BUILD_GTEST ON)

FetchContent_MakeAvailable(googletest)

# ------------------------------
# Cible de tests
# ------------------------------
add_executable(fpencoder_tests
    test_fpencoder.cpp
)

target_link_libraries(fpencoder_tests
    PRIVATE
        fpencoder
        gtest
        gtest_main
)

add_test(NAME fpencoder_tests COMMAND fpencoder_tests)
