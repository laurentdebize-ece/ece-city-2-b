cmake_minimum_required(VERSION 3.20)
project(projet_ece_city C)

set(CMAKE_C_STANDARD 99)

add_executable(projet_ece_city main.c plateau.c plateau.h menuDebut.c menuDebut.h boiteOutils.c boiteOutils.h include.h batiments.h batiments.c batiments.h graphe.c graphe.h bfs.c bfs.h sauvegarde.c sauvegarde.h niveaux.c niveaux.h)

IF(WIN32)
    target_link_libraries(projet_ece_city -lmingw32 -lallegro-5.0.10-monolith-md-debug)
ELSE()
    INCLUDE_DIRECTORIES(/usr/local/Cellar/allegro/5.2.7.0/include)
    LINK_DIRECTORIES(/usr/local/Cellar/allegro/5.2.7.0/lib)
    file(GLOB LIBRARIES "/usr/local/Cellar/allegro/5.2.7.0/lib/*.dylib")
    message("LIBRARIES = ${LIBRARIES}")
    TARGET_LINK_LIBRARIES(projet_ece_city ${LIBRARIES})
ENDIF(WIN32)