;--------------------------------------------------------
; Created by Eduardo Alves da Silva
;--------------------------------------------------------
; Copyright (C) 2015 LEDS - Univali <zeferino@univali.br>
; Laboratory of Embedded and Distributed Systems
; University of Vale do Itajaí
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
[Setup]
AppComments=This application is maintened by LEDS - Univali (Brazil/SC)
AppContact=For more information please enter in contact with zeferino@univali.br
AppCopyright=Copyright (C) 2015 LEDS - Univali
AppPublisher=LEDS - Univali
AppName=RedScarf
AppVersion=1.0
AllowNoIcons=yes
DefaultDirName={pf}\RedScarf
DefaultGroupName=LEDS-Univali
LicenseFile=LICENSE
UninstallDisplayIcon={app}\RedScarf.exe
Compression=lzma2
SolidCompression=yes
SetupIconFile=RedScarf_icon.ico
OutputDir=userdocs:RedScarfAppInstaller
OutputBaseFilename=RS Setup
WizardImageFile=WizardImage.bmp
PrivilegesRequired=admin

[Dirs]
Name: "{app}\etc"
Name: "{app}\translations"
Name: "{app}\system"
Name: "{app}\platforms"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "RedScarf.exe"; DestDir: "{app}"
Source: "*.dll"; DestDir: "{app}"
Source: "etc\*"; DestDir: "{app}\etc"
Source: "system\*"; DestDir: "{app}\system"
Source: "platforms\*"; DestDir: "{app}\platforms"
Source: "translations\*"; DestDir: "{app}\translations"
Source: "Manual.pdf"; DestDir: "{app}"
Source: "NEWS"; DestDir: "{app}"
Source: "ChangeLog"; DestDir: "{app}"
Source: "AUTHORS"; DestDir: "{app}"
Source: "README_RS.txt"; DestDir: "{app}"; Flags: isreadme

[Icons]
Name: "{group}\RedScarf"; Filename: "{app}\RedScarf.exe"
Name: "{group}\RedScarf User Manual"; Filename: "{app}\Manual.pdf"
Name: "{group}\Uninstall RedScarf"; Filename: "{uninstallexe}"
Name: "{commondesktop}\RedScarf"; Filename: "{app}\RedScarf.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\RedScarf"; Filename: "{app}\RedScarf.exe"; Tasks: quicklaunchicon