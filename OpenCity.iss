; ------------------------------------------------------------------------------
; -- OpenCity.iss --
;
; Last modified:
;	$Id$
; ------------------------------------------------------------------------------

[Setup]
AppName=OpenCity 0.0.5 beta
AppVerName=OpenCity 0.0.5 beta
AppPublisherURL=http://opencity.sourceforge.net
AppCopyright=Copyright © 2006 Duong-Khang (neoneurone) NGUYEN
DefaultDirName={pf}\OpenCity
DefaultGroupName=Open City
;UninstallDisplayIcon={app}\MyProg.exe
Compression=lzma
SolidCompression=yes
LicenseFile=COPYING
VersionInfoVersion=0.0.5
WindowVisible=Yes

[Files]
Source: "OpenCity-0.0.5beta.exe"; DestDir: "{app}\bin";
Source: "*.dll"; DestDir: "{app}\bin"
Source: "config\*"; DestDir: "{app}\share\opencity\config"; Excludes: ".svn"
Source: "docs\FAQ.txt"; DestDir: "{app}\share\opencity\docs";
Source: "graphism\*"; DestDir: "{app}\share\opencity\graphism"; Excludes: ".svn"; Flags: recursesubdirs createallsubdirs
Source: "model\*"; DestDir: "{app}\share\opencity\model"; Excludes: ".svn"
Source: "sound\*"; DestDir: "{app}\share\opencity\sound"; Excludes: ".svn"
Source: "texture\*"; DestDir: "{app}\share\opencity\texture"; Excludes: ".svn"
Source: "README.txt"; DestDir: "{app}"; Flags: isreadme

[Icons]
Name: "{group}\OpenCity 0.0.5beta"; Filename: "{app}\bin\OpenCity-0.0.5beta.exe"; Parameters: "--homedir ""{app}\share\opencity"""; WorkingDir: "{app}\share\opencity"
Name: "{group}\FAQ"; Filename: "{app}\share\opencity\docs\FAQ.txt"
Name: "{group}\ReadMe"; Filename: "{app}\README.txt";
Name: "{group}\Uninstall"; Filename: "{uninstallexe}";

[UninstallDelete]
Type: filesandordirs; Name: "{app}"



