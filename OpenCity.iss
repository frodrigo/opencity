; ------------------------------------------------------------------------------
; -- OpenCity.iss --
;
; Last modified:
;	$Id$
; ------------------------------------------------------------------------------

[Setup]
AppName=OpenCity
AppVerName=OpenCity 0.0.4 beta
AppPublisherURL=http://opencity.sourceforge.net
AppCopyright=Copyright © 2006 Duong-Khang (neoneurone) NGUYEN
DefaultDirName={pf}\OpenCity
DefaultGroupName=Open City
;UninstallDisplayIcon={app}\MyProg.exe
Compression=lzma
SolidCompression=yes
LicenseFile=COPYING
WindowVisible=Yes

[Files]
Source: "OpenCity.exe"; DestDir: "{app}\bin"
Source: "*.dll"; DestDir: "{app}\bin"
Source: "config\*"; DestDir: "{app}\share\opencity\config"
Source: "docs\*"; DestDir: "{app}\share\opencity\docs"
Source: "graphism\*"; DestDir: "{app}\share\opencity\graphism"; Flags: recursesubdirs createallsubdirs
Source: "model\*"; DestDir: "{app}\share\opencity\model"
Source: "sound\*"; DestDir: "{app}\share\opencity\sound"
Source: "texture\*"; DestDir: "{app}\share\opencity\texture"
Source: "README.txt"; DestDir: "{app}"; Flags: isreadme

[Icons]
Name: "{group}\OpenCity 0.0.4beta"; Filename: "{app}\bin\opencity.exe"; Parameters: "--homedir ""{app}\share\opencity"""; WorkingDir: "{app}\share\opencity"
Name: "{group}\FAQ"; Filename: "{app}\share\opencity\docs\FAQ.txt"
Name: "{group}\Uninstall"; Filename: "{uninstallexe}";

[UninstallDelete]
Type: filesandordirs; Name: "{app}"



