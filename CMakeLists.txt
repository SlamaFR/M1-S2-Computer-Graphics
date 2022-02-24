cmake_minimum_required(VERSION 3.16)

project("Synthèse d'image")

add_compile_options(-std=c17 -Wall -O2 $ENV{incG3X} $ENV{libG3X})

# TPs
file(GLOB tp_folders TD*)

foreach(tp ${tp_folders})
    add_subdirectory(${tp})
endforeach()
