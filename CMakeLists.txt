cmake_minimum_required(VERSION 3.16)

project(El_Canal VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    prueba.h
    prueba.c  # Asegúrate de incluir esto
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(El_Canal
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(El_Canal SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(El_Canal
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(El_Canal PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Resto de tu CMakeLists.txt...
