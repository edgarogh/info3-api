cmake_minimum_required(VERSION 3.20)
project(info3_api C)

set(CMAKE_C_STANDARD 11)

include_directories(TP_Mesure)

add_executable(mesure_tris
        TP_Mesure/mesure_tris.c
        TP_Mesure/tris.c
        TP_Mesure/tris.h)

target_link_libraries(mesure_tris m)

add_executable(tests
        TP_Mesure/tests.c
        TP_Mesure/tris.c
        TP_Mesure/tris.h)

target_link_libraries(tests m)
