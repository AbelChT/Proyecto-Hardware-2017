cmake_minimum_required(VERSION 3.8)
project(ProyectoHardware)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES Bmp.c Bmp.h lcd.c lcd.h main.c lcdManager.h lcdManager.c)
add_library(ProyectoHardware ${SOURCE_FILES})