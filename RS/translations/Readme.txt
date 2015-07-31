===============================================================================
--> Directory: (root)/RS/translations
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
| This directory contains the RedScarf's front-end files to be translated.
===============================================================================

===============================================================================
| In this directory are the files of the translations to be made.
| Steps to translate:
| -----------------------------------------------------------------------------
| 1-) Codify the "strings" in the source-codes inside of method trUtf8("...") or
|     in some available variation that allow the translation.
|
| 2-) To create/update the translation files, use Qt tool "lupdate" in the project
|     directory ("(root)/RS"). The translation files (.ts) will be allocated here.
|
| 3-) Use the Qt tool "linguist" to translate the "strings".
|
| 4-) Use the Qt tool "lrelease" (available in linguist) to generate the translated
|     files that will be used by application.
|
| 5-) Put the translated generated files (.qm) in directory:
|    "(root)/RS/bin/translations".
|
| 6-) Verify if the translation is indicated in the configuration file:
|    "(root)/RS/bin/etc/config.ini". NOTE: Don't use .qm in indication.
|
| 7-) Test on application.
===============================================================================

===============================================================================
| NOTE: The translation indication in config.ini file, must be taken on section
| "[AvailableLanguages]". Can be specified one or more files to same translation,
| provided they are not conflicting (translate the same string from same local).
| In section [AvailableLanguages] two attributes are defined, that are:
| * n\Files: -> translation files (.qm) without extension. Separate with comma.
| * n\Language: -> Text to view and select the language on graphical interface.
|                  NOTE: Use UTF-8 codification to define this text.
|
| Don't forget of increment the attribute "size" from this section if a new
| language is added.
===============================================================================
