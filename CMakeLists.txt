cmake_minimum_required(VERSION 3.19)
project(QtADCOscilloscope LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(QtADCOscilloscope
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Display/CWaveformWidget.h
    Display/CWaveformWidget.cpp
    ADC/CADCInterface.h
    ADC/CADCPlugin.h
    ADC/CADCPlugin.cpp
    ADC/CADCInterface.h
    ADC/CADCPlugin.h
    ADC/CADCPlugin.cpp
)

target_include_directories(QtADCOscilloscope
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(QtADCOscilloscope
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS QtADCOscilloscope
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET QtADCOscilloscope
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
