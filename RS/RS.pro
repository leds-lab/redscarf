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
    Model/Traffic/MathFunction.cpp \
    Model/Traffic/TrafficModelGenerator.cpp \
    Model/System/Defines.cpp \
    Model/System/Experiment.cpp \
    Model/System/SystemDefines.cpp \
    Model/Analysis/DataReport.cpp \
    Model/Analysis/PacketInfo.cpp \
    Model/Analysis/PerformanceAnalysis.cpp \
    Model/Analysis/TrafficAnalysis.cpp \
    Model/Analysis/ReportReader.cpp \
    View/AnalysisOptions.cpp \
    View/ConfigDialog.cpp \
    View/Pages.cpp \
    View/AboutWindow.cpp \
    View/GetSelectedItemsDialog.cpp \
    View/ReportDialog.cpp \
    View/MainWindow.cpp \
    View/Plotter.cpp \
    View/CustomPlotter.cpp \
    Control/ThreadManager.cpp \
    Control/WaveformViewer.cpp \
    Control/Analyzer.cpp \
    Control/FolderCompressor.cpp \
    Control/EnvironmentConfiguration.cpp \
    Control/ExternalWaveformViewer.cpp \
    Control/Control.cpp \
    Control/SimulationPerformer.cpp \
#    Control/XmlConfigParser.cpp \
    Main.cpp \
    View/qcustomplot.cpp \
    View/CustomPlotZoom.cpp \
    Model/FolderOperation.cpp \
    Model/TimeOperation.cpp \
    View/Arc.cpp \
    View/Edge.cpp \
    View/PersonalGraphicsView.cpp \
    View/TrafficConfigurationDialog.cpp \
    View/TrafficEditDialog.cpp \
    View/Vertice.cpp \
    Model/Traffic/SpatialDistribution.cpp \
    Model/Traffic/TrafficParameters.cpp \
    Model/System/SystemParameters.cpp \
    Model/System/SystemOperation.cpp

# Application Headers files
HEADERS += \
    Model/Traffic/MathFunction.h \
    Model/Traffic/TrafficModelGenerator.h \
    Model/System/Defines.h \
    Model/System/Experiment.h \
    Model/System/SystemDefines.h \
    Model/Analysis/DataReport.h \
    Model/Analysis/PacketInfo.h \
    Model/Analysis/PerformanceAnalysis.h \
    Model/Analysis/TrafficAnalysis.h \
    Model/Analysis/ReportReader.h \
    View/AnalysisOptions.h \
    View/ConfigDialog.h \
    View/Pages.h \
    View/AboutWindow.h \
    View/GetSelectedItemsDialog.h \
    View/ReportDialog.h \
    View/MainWindow.h \
    View/Plotter.h \
    View/CustomPlotter.h \
    Control/ThreadManager.h \
    Control/WaveformViewer.h \
    Control/Analyzer.h \
    Control/FolderCompressor.h \
    Control/EnvironmentConfiguration.h \
    Control/ExternalWaveformViewer.h \
    Control/Control.h \
    Control/SimulationPerformer.h \
#    Control/XmlConfigParser.h \
    Main.h \
    View/qcustomplot.h \
    View/CustomPlotZoom.h \
    Model/FolderOperation.h \
    Model/TimeOperation.h \
    View/Arc.h \
    View/Edge.h \
    View/PersonalGraphicsView.h \
    View/TrafficConfigurationDialog.h \
    View/TrafficEditDialog.h \
    View/Vertice.h \
    Model/Traffic/SpatialDistribution.h \
    Model/Traffic/TrafficParameters.h \
    Model/System/SystemParameters.h \
    Model/System/SystemOperation.h

# Application configurations support
CONFIG += qt
CONFIG += exceptions
CONFIG += thread
CONFIG += c++11

# Graphics forms (.ui)
FORMS  += \
    View/AboutWindow.ui \
    View/PreviewDialog.ui \
    View/FoldersConfigurationPage.ui \
    View/GeneralConfigurationPage.ui \
    View/MainWindow.ui \
    View/Plotter.ui \
    View/TrafficConfigurationDialog.ui \
    View/TrafficEditDialog.ui

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
