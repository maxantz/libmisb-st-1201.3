cmake_minimum_required(VERSION 3.14)

# Nom du projet
project(misb-st-1201.3 LANGUAGES CXX)

# Choix du standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------
#          Bibliothèque principale
# ---------------------------------------

# Récupération automatique des sources
file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# Include
include_directories("${PROJECT_SOURCE_DIR}/include")

# Création de la bibliothèque dynamique
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Inclure le dossier courant pour les headers
# target_include_directories(${PROJECT_NAME} PUBLIC
#     "${CMAKE_CURRENT_SOURCE_DIR}/include"
# )

# Pour assurer une bonne position indépendante du code (utile en shared lib)
set_target_properties(${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# ---------------------------------------
#          Tests unitaires
# ---------------------------------------

# Option pour activer/désactiver les tests
option(ENABLE_TESTING "Compile and run unit tests" ON)

if (ENABLE_TESTING)
    include(CTest)
    enable_testing()

    # Télécharger GoogleTest automatiquement
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    FetchContent_MakeAvailable(googletest)

    # Exclure GoogleTest des cibles installables
    set_property(TARGET gtest PROPERTY EXCLUDE_FROM_ALL TRUE)
    set_property(TARGET gtest_main PROPERTY EXCLUDE_FROM_ALL TRUE)
    set_property(TARGET gmock PROPERTY EXCLUDE_FROM_ALL TRUE)
    set_property(TARGET gmock_main PROPERTY EXCLUDE_FROM_ALL TRUE)

    # Récupération des fichiers de test
    file(GLOB_RECURSE TEST_SOURCES
        "${PROJECT_SOURCE_DIR}/TU/*.cpp"
    )

    # Création de l'exécutable de test
    add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})

    # Inclusion du dossier include/
    target_include_directories(${PROJECT_NAME}_tests PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/TU/include"
    )

    # Link tests avec : la lib + gtest
    target_link_libraries(${PROJECT_NAME}_tests
        PRIVATE
            ${PROJECT_NAME}
            GTest::gtest
            GTest::gtest_main
    )

    # Ajout automatique dans CTest
    add_test(NAME RunUnitTests COMMAND ${PROJECT_NAME}_tests)
endif()

# ---------------------------------------
#       Mode Release par défaut
# ---------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ---------------------------------------
# Installation
# ---------------------------------------
install(TARGETS ${PROJECT_NAME}
        EXPORT "${PROJECT_NAME}-config"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
        PRIVATE_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")

install(EXPORT "${PROJECT_NAME}-config" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

export(TARGETS ${PROJECT_NAME} FILE "${PROJECT_NAME}-exports.cmake")

