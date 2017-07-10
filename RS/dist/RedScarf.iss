;--------------------------------------------------------
; Created by Eduardo Alves da Silva
;--------------------------------------------------------
; Copyright (C) 2015-2107 LEDS - Univali <zeferino@univali.br>
; Laboratory of Embedded and Distributed Systems
; University of Vale do Itajai 
;--------------------------------------------------------
;
; -- RedScarf.iss --
; This script generates a windows installer to RedScarf and
; copy all library dependencies inside of dist folder.
;
; Use Inno Setup tool
;
; ATTENTION: Verify all needed files on "Files" section to build the installer.
;
#ifdef x64
 #define 
#else

#endif

[Setup]
AppComments=This application is maintened by LEDS - Univali (Brazil/SC)
AppContact=For more information please enter in contact with zeferino@univali.br
AppCopyright=Copyright (C) 2015-2017 LEDS - Univali
AppPublisher=LEDS - Univali
AppName=RedScarf
AppVersion=2.0
AllowNoIcons=yes
DefaultDirName={pf}\RedScarf
DefaultGroupName=LEDS-Univali
UninstallDisplayIcon={app}\RedScarf.exe
Compression=lzma2
SolidCompression=yes
SetupIconFile=RedScarf_icon.ico
OutputDir=userdocs:RedScarfAppInstaller
OutputBaseFilename=RS Setup
WizardImageFile=WizardImage.bmp
PrivilegesRequired=admin
ArchitecturesInstallIn64BitMode=x64

[Dirs]
Name: "{app}\etc"
Name: "{app}\translations"
Name: "{app}\platforms"
Name: "{app}\SoCIN_Simulator\plugins"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; x64
Source: "RedScarf_x64\RedScarf.exe"; DestDir: "{app}"; Check: Is64BitInstallMode
Source: "RedScarf_x64\*.dll"; DestDir: "{app}"; Check: Is64BitInstallMode
Source: "RedScarf_x64\etc\*"; DestDir: "{app}\etc"; Check: Is64BitInstallMode
Source: "RedScarf_x64\platforms\*"; DestDir: "{app}\platforms"; Check: Is64BitInstallMode
Source: "RedScarf_x64\translations\*"; DestDir: "{app}\translations"; Check: Is64BitInstallMode
; x86 
Source: "RedScarf_x86\RedScarf.exe"; DestDir: "{app}"; Check: not Is64BitInstallMode; Flags: solidbreak
Source: "RedScarf_x86\*.dll"; DestDir: "{app}"; Check: not Is64BitInstallMode
Source: "RedScarf_x86\etc\*"; DestDir: "{app}\etc"; Check: not Is64BitInstallMode
Source: "RedScarf_x86\platforms\*"; DestDir: "{app}\platforms"; Check: not Is64BitInstallMode
Source: "RedScarf_x86\translations\*"; DestDir: "{app}\translations"; Check: not Is64BitInstallMode
; Common
Source: "SoCIN_Simulator\*"; DestDir: "{app}\SoCIN_Simulator"; Flags: solidbreak
Source: "SoCIN_Simulator\plugins\*"; DestDir: "{app}\SoCIN_Simulator\plugins";
Source: "NEWS"; DestDir: "{app}"
Source: "ChangeLog"; DestDir: "{app}"
Source: "AUTHORS"; DestDir: "{app}"
Source: "README_RS.txt"; DestDir: "{app}"; Flags: isreadme

[Icons]
Name: "{group}\RedScarf"; Filename: "{app}\RedScarf.exe"
Name: "{group}\Uninstall RedScarf"; Filename: "{uninstallexe}"
Name: "{commondesktop}\RedScarf"; Filename: "{app}\RedScarf.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\RedScarf"; Filename: "{app}\RedScarf.exe"; Tasks: quicklaunchicon