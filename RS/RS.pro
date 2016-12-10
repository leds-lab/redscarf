#-------------------------------------------------
#
# Project created by Eduardo Alves da Silva
#
#-------------------------------------------------
#
# RS.pro
# Copyright (C) 2014 - 2016 LEDS - Univali <zeferino@univali.br>
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
# ----------------------------------------------------------------------------
# 09/07/2016 - 1.1     - Eduardo Alves da Silva      | First refactoring
# ----------------------------------------------------------------------------
# 09/12/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
# ----------------------------------------------------------------------------
#


# Qt Modules
QT       = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport # Qt 5


# Template default for applications
TEMPLATE = app

# Folder binary destination
DESTDIR = bin

TARGET = RedScarf

# Application version
VERSION = 2.0


# Folders for compiled files (include pre-compiled)
UI_DIR      = $$_PRO_FILE_PWD_/build/ui_files    # UI header files (.h) generated by .ui (forms)
MOC_DIR     = $$_PRO_FILE_PWD_/build/moc_files   # Qt generated files (by modules) (.cpp)
RCC_DIR     = $$_PRO_FILE_PWD_/build/res_files   # Resources files (.cpp) generated by .qrc
OBJECTS_DIR = $$_PRO_FILE_PWD_/build/objs        # C++ object files (.o)


# Application Sources files
SOURCES +=  \
    src/Model/Traffic/MathFunction.cpp \
    src/Model/Traffic/TrafficModelGenerator.cpp \
    src/Model/System/Defines.cpp \
    src/Model/System/Experiment.cpp \
    src/Model/System/SystemDefines.cpp \
    src/Model/Analysis/DataReport.cpp \
    src/Model/Analysis/PacketInfo.cpp \
    src/Model/Analysis/PerformanceAnalysis.cpp \
    src/Model/Analysis/TrafficAnalysis.cpp \
    src/Model/Analysis/ReportReader.cpp \
    src/View/AnalysisOptions.cpp \
    src/View/ConfigDialog.cpp \
    src/View/Pages.cpp \
    src/View/AboutWindow.cpp \
    src/View/GetSelectedItemsDialog.cpp \
    src/View/ReportDialog.cpp \
    src/View/MainWindow.cpp \
    src/View/Plotter.cpp \
    src/View/CustomPlotter.cpp \
    src/Control/ThreadManager.cpp \
    src/Control/WaveformViewer.cpp \
    src/Control/Analyzer.cpp \
    src/Control/FolderCompressor.cpp \
    src/Control/EnvironmentConfiguration.cpp \
    src/Control/ExternalWaveformViewer.cpp \
    src/Control/Control.cpp \
    src/Control/SimulationPerformer.cpp \
#    src/Control/XmlConfigParser.cpp \
    src/Main.cpp \
    src/View/qcustomplot.cpp \
    src/View/CustomPlotZoom.cpp \
    src/Model/FolderOperation.cpp \
    src/Model/TimeOperation.cpp \
    src/View/Arc.cpp \
    src/View/Edge.cpp \
    src/View/PersonalGraphicsView.cpp \
    src/View/TrafficConfigurationDialog.cpp \
    src/View/TrafficEditDialog.cpp \
    src/View/Vertice.cpp \
    src/Model/Traffic/SpatialDistribution.cpp \
    src/Model/Traffic/TrafficParameters.cpp \
    src/Model/System/SystemParameters.cpp \
    src/Model/System/SystemOperation.cpp

# Application Headers files
HEADERS += \
    include/Model/Traffic/MathFunction.h \
    include/Model/Traffic/TrafficModelGenerator.h \
    include/Model/System/Defines.h \
    include/Model/System/Experiment.h \
    include/Model/System/SystemDefines.h \
    include/Model/Analysis/DataReport.h \
    include/Model/Analysis/PacketInfo.h \
    include/Model/Analysis/PerformanceAnalysis.h \
    include/Model/Analysis/TrafficAnalysis.h \
    include/Model/Analysis/ReportReader.h \
    include/View/AnalysisOptions.h \
    include/View/ConfigDialog.h \
    include/View/Pages.h \
    include/View/AboutWindow.h \
    include/View/GetSelectedItemsDialog.h \
    include/View/ReportDialog.h \
    include/View/MainWindow.h \
    include/View/Plotter.h \
    include/View/CustomPlotter.h \
    include/Control/ThreadManager.h \
    include/Control/WaveformViewer.h \
    include/Control/Analyzer.h \
    include/Control/FolderCompressor.h \
    include/Control/EnvironmentConfiguration.h \
    include/Control/ExternalWaveformViewer.h \
    include/Control/Control.h \
    include/Control/SimulationPerformer.h \
#    include/Control/XmlConfigParser.h \
    include/Main.h \
    include/View/qcustomplot.h \
    include/View/CustomPlotZoom.h \
    include/Model/FolderOperation.h \
    include/Model/TimeOperation.h \
    include/View/Arc.h \
    include/View/Edge.h \
    include/View/PersonalGraphicsView.h \
    include/View/TrafficConfigurationDialog.h \
    include/View/TrafficEditDialog.h \
    include/View/Vertice.h \
    include/Model/Traffic/SpatialDistribution.h \
    include/Model/Traffic/TrafficParameters.h \
    include/Model/System/SystemParameters.h \
    include/Model/System/SystemOperation.h

# Application configurations support
CONFIG += qt
CONFIG += exceptions
CONFIG += thread
CONFIG += c++11

# Graphics forms (.ui)
FORMS  += \
    forms/AboutWindow.ui \
    forms/PreviewDialog.ui \
    forms/FoldersConfigurationPage.ui \
    forms/GeneralConfigurationPage.ui \
    forms/MainWindow.ui \
    forms/Plotter.ui \
    forms/TrafficConfigurationDialog.ui \
    forms/TrafficEditDialog.ui

# Application resources files (.qrc)
RESOURCES += resources/Icons.qrc

# OS X specific settings
mac {
    ICON = resources/icons/RedScarf_icon.icns
    FRAMEWORK_HEADERS.version = $$[VERSION]
    QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
}

# Win32 specific settings
win32 {
    QMAKE_TARGET_COMPANY = "LEDS - Laboratory of Embedded and Distributed Systems"
    QMAKE_TARGET_PRODUCT = RedScarf
    QMAKE_TARGET_DESCRIPTION = "Network-on-Chip Simulator"
    QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2014 - 2017 LEDS - Univali"
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
    DEFINES += QT_NO_DEBUG_OUTPUT
    DEFINES -= QT_NO_WARNING_OUTPUT
    DEFINES += QT_NO_DEBUG
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
