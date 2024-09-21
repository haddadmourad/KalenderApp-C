cmake_minimum_required(VERSION 3.27)
project(KalenderApp_C)

set(CMAKE_CXX_STANDARD 14)

include_directories("without warnings")

add_executable(KalenderApp_C
        .vscode/c_cpp_properties.json
        .vscode/launch.json
        .vscode/settings.json
        "without warnings/a.exe"
        "without warnings/app.c"
        "without warnings/au.exe"
        "without warnings/myDateFunctions.c"
        "without warnings/myDateFunctions.h"
        "without warnings/prak5.c"
        "without warnings/prak5.h"
        "without warnings/stundenplan.c"
        "without warnings/stundenplan.h"
        "without warnings/termine.txt"
        .gitattributes
        README.md)
