===============================================================================
--> Directory: (root)/RS/bin/
===============================================================================
------------------------------------------------------------------------------|
| Generated on 15/07/2015
| Initial release
| Version 1.0.0
| Author: Eduardo Alves da Silva
| --------- Changes ---------
| Last update: 15/07/2015
| By: Eduardo Alves da Silva
| --------- Details ---------
| Ver. 1.0.0: File created
|
------------------------------------------------------------------------------|

===============================================================================
| This directory contains the RedScarf's distribution files.
===============================================================================

===============================================================================
| In this directory the front-end's executable is created (Windows: .exe &
| Linux: elf).
| NOTE: In OS X, the application Bundle is created and within it is the
| front-end executable.
|
| The distribution subdirs are: "etc", "system" e "translations".
| These subdirs must be in the same executable directory, because the application
| use the files in subdirs.
| The following describes the contents of each subdirectory.
| -----------------------------------------------------------------------------
| * etc          : Place that contains the front-end's configuration files.
|                  The files from this directory are:
|                - config.ini: It has the definitions of available languages,
|                              the number of simulation threads and the environment
|                              directories setting.
|                - DefaultConfiguration.drsf: File in XML format that contains
|                              the environment's parameterization.
|                              NOTE: If this file don't exists, it is generated on
|                              first front-end execution.
|                - system.ini: It contains definitions of the back-end models
|                              to be compiled, the configuration definitions
|                              from the models on front-end and possibility use
|                              of alternative traffic file.
| -----------------------------------------------------------------------------
| * system       : Place that contains the environment's back-end, that are the
|                  SystemC models(.h e .cpp) of the components to be simulated.
| -----------------------------------------------------------------------------
| * translations : Place of translation files of the front-end.
===============================================================================

===============================================================================
| IMPORTANT: In subdirs "etc" and "system" the user must have write permission,
| because the configuration files are updated by front-end and the object
| files of the back-end's compilation are generated on directory of models.
===============================================================================

===============================================================================
| NOTICE: For OS X users, the subdirs must be allocated within of application
| Bundle (the Bundle is the file RedScarf.app, generated on compilation of
| front-end). The place to allocate the subdirs is:
|     ->    "RedScarf.app/Contents/MacOS/"
| This way, the subdirs are at the same executable place in OS X.
===============================================================================

===============================================================================
| NOTE: To release the tool, Qt and Qwt libraries should be included in the
| distribution package.
| * In Windows are the dynamic link libraries (.dll) that they can be obtained
|   by the "windeployqt" tool (copy dlls other than those required) from Qt.
|
| * In Linux are the shared objects (.so) and they can be resolved by package
|   manager ("apt-get", "yum", ...). It is recommended copy the Qwt (maybe some
|   package manager doesn't resolve this library and/or cause conflict of
|   versions).
|
| * In OS X are the dynamic libraries (.dylib) that they can be incorporated to
|   Bundle with "macdeployqt" tool (copy dylibs to within of Bundle) from Qt.
|   To generate a disc image, use the argument -dmg with macdeployqt.
|
| For more details about dependencies, look the "Requirements.txt" file in
| project root folder.
===============================================================================
