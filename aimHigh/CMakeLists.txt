cmake_minimum_required(VERSION 3.16)

project(aimHigh VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Quick Network Core OpenGLWidgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Quick Network Core OpenGLWidgets)

# Define translation source files (.ts)
set(TS_FILES
    ${CMAKE_SOURCE_DIR}/i18n/aimHigh_en.ts
    ${CMAKE_SOURCE_DIR}/i18n/aimHigh_pl.ts
)

# Add translations (.qm files will be created from .ts)
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

# Add resource file with embedded .qm
qt_add_resources(RESOURCES
    "resources.qrc"
)

# Define project sources
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    apihandler.cpp
    apihandler.h
    ${RESOURCES}
)

# Add the executable
qt_add_executable(aimHigh
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    earthwidget.h earthwidget.cpp
)

# Link libraries
target_link_libraries(aimHigh
    PRIVATE
    Qt6::Widgets
    Qt6::Network
    Qt6::Gui
    Qt6::Quick
    Qt6::OpenGLWidgets
    OpenGL::GL
)

# Bundle settings
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.aimHigh)
endif()
set_target_properties(aimHigh PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install rules
include(GNUInstallDirs)
install(TARGETS aimHigh
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize executable (embed translations)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(aimHigh ${QM_FILES})
endif()
