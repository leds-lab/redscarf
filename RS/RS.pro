#-------------------------------------------------
#
# Project created by Eduardo Alves da Silva
#
#-------------------------------------------------
#
# RS.pro
# Copyright (C) 2014 LEDS - Univali <zeferino@univali.br>
# Laboratory of Embedded and Distributed Systems
# University of Vale do Itajaí
#
# Authors: Laboratory of Embedded and Distributed Systems (LEDS)
#
# Contact: Profº Dr. Cesar Albenes Zeferino {zeferino@univali.br}
#
# RedScarf is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# RedScarf is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# ------------------------------ Reviews -------------------------------------
# Date       - Version - Author                      | Description
# ----------------------------------------------------------------------------
# 10/12/2014 - 1.0     - Eduardo Alves da Silva      | Initial release
#
#


# Qt Modules
QT       = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets # Qt 5


# Template default for applications
TEMPLATE = app

# Folder binary destination
DESTDIR = bin

TARGET = RedScarf

# Application version
VERSION = 1.0


# Folders for compiled files (include pre-compiled)
UI_DIR      = $$_PRO_FILE_PWD_/build/ui_files    # UI header files (.h) generated by .ui (forms)
MOC_DIR     = $$_PRO_FILE_PWD_/build/moc_files   # Qt generated files (by modules) (.cpp)
RCC_DIR     = $$_PRO_FILE_PWD_/build/res_files   # Resources files (.cpp) generated by .qrc
OBJECTS_DIR = $$_PRO_FILE_PWD_/build/objs        # C++ object files (.o)


# Application Sources files
SOURCES +=  \
    src/Model/Util.cpp \
    src/Model/Traffic/MathFunction.cpp \
    src/Model/Traffic/Node.cpp \
    src/Model/Traffic/TrafficModelGenerator.cpp \
    src/Model/Traffic/TrafficParameters.cpp \
    src/Model/Traffic/TrafficPatternDefines.cpp \
    src/Model/Traffic/TrafficPatternManager.cpp \
    src/Model/System/Defines.cpp \
    src/Model/System/Experiment.cpp \
    src/Model/System/GSoC.cpp \
    src/Model/System/GSoCIN2VcStrategy.cpp \
    src/Model/System/GSoCINStrategy.cpp \
    src/Model/System/SoCINGenerationStrategy.cpp \
    src/Model/System/SystemParameters.cpp \
    src/Model/System/SystemDefines.cpp \
    src/Model/System/ExperimentManager.cpp \
    src/Model/Analysis/DataReport.cpp \
    src/Model/Analysis/PacketInfo.cpp \
    src/Model/Analysis/PerformanceAnalysis.cpp \
    src/Model/Analysis/TrafficAnalysis.cpp \
    src/View/DefaultPreviewTrafficConfiguration.cpp \
    src/View/XMLPreviewTrafficConfiguration.cpp \
    src/View/AnalysisOptions.cpp \
    src/View/QwtPlotter.cpp \
    src/View/PersonalQwtPlotPicker.cpp \
    src/View/ConfigDialog.cpp \
    src/View/Pages.cpp \
    src/View/AboutWindow.cpp \
    src/View/GetSelectedItemsDialog.cpp \
    src/View/ReportDialog.cpp \
    src/View/TrafficConfigurationDialog.cpp \
    src/View/MainWindow.cpp \
    src/Control/Builder.cpp \
    src/Control/GnuPlotPlotter.cpp \
    src/Control/Plotter.cpp \
    src/Control/ThreadManager.cpp \
    src/Control/WaveformViewer.cpp \
    src/Control/Analyzer.cpp \
    src/Control/FolderCompressor.cpp \
    src/Control/EnvironmentConfiguration.cpp \
    src/Control/ExternalWaveformViewer.cpp \
    src/Control/Control.cpp \
    src/Control/SimulationPerformer.cpp \
    src/Main.cpp

# Application Headers files
HEADERS += \
    include/Model/Util.h \
    include/Model/Traffic/MathFunction.h \
    include/Model/Traffic/Node.h \
    include/Model/Traffic/TrafficModelGenerator.h \
    include/Model/Traffic/TrafficParameters.h \
    include/Model/Traffic/TrafficPatternDefines.h \
    include/Model/Traffic/TrafficPatternManager.h \
    include/Model/System/Defines.h \
    include/Model/System/Experiment.h \
    include/Model/System/GNoC.h \
    include/Model/System/GSoC.h \
    include/Model/System/GSoCIN2VcStrategy.h \
    include/Model/System/GSoCINStrategy.h \
    include/Model/System/SoCINGenerationStrategy.h \
    include/Model/System/SystemParameters.h \
    include/Model/System/SystemDefines.h \
    include/Model/System/ExperimentManager.h \
    include/Model/Analysis/DataReport.h \
    include/Model/Analysis/PacketInfo.h \
    include/Model/Analysis/PerformanceAnalysis.h \
    include/Model/Analysis/TrafficAnalysis.h \
    include/View/AnalysisOptions.h \
    include/View/DefaultPreviewTrafficConfiguration.h \
    include/View/XMLPreviewTrafficConfiguration.h \
    include/View/QwtPlotter.h \
    include/View/PersonalQwtPlotPicker.h \
    include/View/ConfigDialog.h \
    include/View/Pages.h \
    include/View/AboutWindow.h \
    include/View/GetSelectedItemsDialog.h \
    include/View/ReportDialog.h \
    include/View/PreviewDialog.h \
    include/View/TrafficConfigurationDialog.h \
    include/View/MainWindow.h \
    include/Control/Builder.h \
    include/Control/GnuPlotPlotter.h \
    include/Control/Plotter.h \
    include/Control/ThreadManager.h \
    include/Control/WaveformViewer.h \
    include/Control/Analyzer.h \
    include/Control/FolderCompressor.h \
    include/Control/EnvironmentConfiguration.h \
    include/Control/ExternalWaveformViewer.h \
    include/Control/Control.h \
    include/Control/SimulationPerformer.h \
    include/Main.h

# Application configurations support
CONFIG += qt
CONFIG += exceptions
CONFIG += thread

# Qwt graphics root folder
QWT_PATH =
isEmpty(QWT_PATH){
# Show error message if QWT_PATH is no defined
    error("QWT library path not defined in file RS.pro line 160")
} else {
# Show a message with QWT_PATH defined
    message(Qwt Path: $$QWT_PATH)
}
include($${QWT_PATH}/features/qwt.prf)


#DEPENDPATH += $${QWT_PATH}/lib
#mac {
#    INCLUDEPATH += $${QWT_PATH}/lib/qwt.framework/Headers/
#    LIBS += -F$${QWT_PATH}/lib -framework qwt
#} else {
#    INCLUDEPATH += $${QWT_PATH}/include
#    LIBS += -L$${QWT_PATH}/lib -lqwt
#}

# Graphics forms (.ui)
FORMS  += \
    forms/AboutWindow.ui \
    forms/PreviewDialog.ui \
    forms/TrafficConfigurationDialog.ui \
    forms/FoldersConfigurationPage.ui \
    forms/GeneralConfigurationPage.ui \
    forms/MainWindow.ui

# Application resources files (.qrc)
RESOURCES += resources/Icons.qrc

# OS X specific settings
mac {
    ICON = resources/icons/RedScarf_icon.icns
    FRAMEWORK_HEADERS.version = "1.0"
    QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
}

# Win32 specific settings
win32 {
    QMAKE_TARGET_COMPANY = "LEDS - Laboratory of Embedded and Distributed Systems"
    QMAKE_TARGET_PRODUCT = RedScarf
    QMAKE_TARGET_DESCRIPTION = "Network-on-Chip Simulator"
    QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2015 LEDS - Univali"
    RC_ICONS = resources/icons/RedScarf_icon.ico
}

# Translations
TRANSLATIONS = translations/RedScarf_pt.ts \
               translations/RedScarf_en.ts

# It don't generates debug and release folders on Windows
CONFIG -= debug_and_release

# Debug define
CONFIG(debug, debug|release) {
    # Application target for debug
    TARGET  = RedScarf_d
    CONFIG -= debug release
    CONFIG += debug
}
# Release define
CONFIG(release, debug|release) {
    # Application target for release
    TARGET  = RedScarf
    CONFIG -= debug release
    CONFIG += release
#    DEFINES += QT_NO_DEBUG_OUTPUT
#    DEFINES -= QT_NO_WARNING_OUTPUT
#    DEFINES += QT_NO_DEBUG
}

# Debug options. Active methods traceability
debug {
    CONFIG  += warn_on
    DEFINES += DEBUG_POINTS_METHODS

    message(Qt version: $$[QT_VERSION])
    message(Qt is installed in $$[QT_INSTALL_PREFIX])
    message(Qt resources can be found in the following locations:)
    message(Documentation: $$[QT_INSTALL_DOCS])
    message(Header files: $$[QT_INSTALL_HEADERS])
    message(Libraries: $$[QT_INSTALL_LIBS])
    message(Binary files (executables): $$[QT_INSTALL_BINS])
    message(Plugins: $$[QT_INSTALL_PLUGINS])
    message(Data files: $$[QT_INSTALL_DATA])
    message(Translation files: $$[QT_INSTALL_TRANSLATIONS])
    message(Settings: $$[QT_INSTALL_CONFIGURATION])
    message(Examples: $$[QT_INSTALL_EXAMPLES])
    message(Project File: $$_PRO_FILE_)
    message(Project File Working Directory: $$_PRO_FILE_PWD_)
}

DISTFILES += \
    bin/etc/config.ini \
    bin/etc/system.ini