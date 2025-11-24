cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(fpencoder CXX)

# Crée une bibliothèque dynamique à partir de fpencoder.cpp
add_library(fpencoder SHARED
    fpencoder.cpp
    fpencoder.h
)

# Spécifie la version de C++ minimale
target_compile_features(fpencoder PUBLIC cxx_std_17)

# Inclure le dossier courant pour les headers
target_include_directories(fpencoder PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Pour assurer une bonne position indépendante du code (utile en shared lib)
set_target_properties(fpencoder PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Optionnel : définir le nom de sortie (sinon ce sera libfpencoder.so/.dll/.dylib)
set_target_properties(fpencoder PROPERTIES OUTPUT_NAME "fpencoder")

# ------------------------------
# Ajout des tests GoogleTest
# ------------------------------
option(FPENCODER_ENABLE_TESTS "Build unit tests" ON)

if (FPENCODER_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
