; ------------------------------------------------------------------------------
; -- OpenCity.iss --
;
; Last modified:
;	$Id$
; ------------------------------------------------------------------------------

[Setup]
AppCopyright=Copyright (C) 2008 by Duong Khang NGUYEN
AppName=OpenCity 0.0.6 beta
AppPublisher=Duong Khang NGUYEN
AppPublisherURL=http://www.opencity.info
AppVerName=OpenCity 0.0.6 beta
AppVersion=OpenCity 0.0.6 beta
Compression=lzma
DefaultDirName={pf}\OpenCity
DefaultGroupName=Open City
LicenseFile=..\COPYING
SolidCompression=yes
VersionInfoVersion=0.0.6
WindowVisible=Yes

[Files]
Source: "bin\Release\OpenCity-0.0.6beta.exe"; DestDir: "{app}\bin";
Source: "bin\Release\*.dll"; DestDir: "{app}\bin"
Source: "..\config\*"; DestDir: "{app}\etc\opencity\config"; Excludes: ".svn"
Source: "..\docs\*.txt"; DestDir: "{app}\share\opencity\docs";
Source: "..\graphism\*"; DestDir: "{app}\share\opencity\graphism"; Excludes: ".svn"; Flags: recursesubdirs createallsubdirs
Source: "..\sound\*"; DestDir: "{app}\share\opencity\sound"; Excludes: ".svn"
Source: "..\README.txt"; DestDir: "{app}"; Flags: isreadme
; MSVC C runtime manifest
;Source: "Release\Microsoft.VC80.CRT.manifest"; DestDir: "{app}\bin"

[Dirs]
Name: "{userappdata}\opencity\"

[Icons]
Name: "{group}\OpenCity 0.0.6beta - window"; Filename: "{app}\bin\OpenCity-0.0.6beta.exe"; IconFilename: "{app}\share\opencity\graphism\icon\OpenCity32.ico"; Parameters: "--data-dir ""{app}\share\opencity"" --conf-dir ""{app}\etc\opencity"""; WorkingDir: "{app}"
Name: "{group}\OpenCity 0.0.6beta - fullscreen (autodetect mode)"; Filename: "{app}\bin\OpenCity-0.0.6beta.exe"; IconFilename: "{app}\share\opencity\graphism\icon\OpenCity32.ico"; Parameters: "--data-dir ""{app}\share\opencity"" --conf-dir ""{app}\etc\opencity"" --full-screen"; WorkingDir: "{app}"
Name: "{group}\Configuration file"; Filename: "{app}\etc\opencity\config\opencity.xml"
Name: "{group}\FAQ"; Filename: "{app}\share\opencity\docs\FAQ.txt"
Name: "{group}\ReadMe"; Filename: "{app}\README.txt";
Name: "{group}\Save folder"; Filename: "{userappdata}\opencity\"
Name: "{group}\Documentation\FAQ in Italian"; Filename: "{app}\share\opencity\docs\FAQ_it.txt"
Name: "{group}\Documentation\INSTALL in Italian"; Filename: "{app}\share\opencity\docs\INSTALL_it.txt"
Name: "{group}\Documentation\README in Italian"; Filename: "{app}\share\opencity\docs\README_it.txt"
Name: "{group}\Documentation\README in Spanish"; Filename: "{app}\share\opencity\docs\README_es.txt"
Name: "{group}\Uninstall"; Filename: "{uninstallexe}";

[UninstallDelete]
Type: filesandordirs; Name: "{app}"

[Run]
Filename: "{win}\explorer.exe"; Parameters: "http://www.opencity.info"; Flags: nowait skipifdoesntexist

