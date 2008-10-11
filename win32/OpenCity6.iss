; ------------------------------------------------------------------------------
; -- OpenCity.iss --
;
; Last modified:
;	$Id$
; ------------------------------------------------------------------------------

[Setup]
AppCopyright=Copyright (C) 2008 by Duong Khang NGUYEN
AppName=OpenCity 0.0.6 beta4
AppPublisher=Duong Khang NGUYEN
AppPublisherURL=http://www.opencity.info
AppVerName=OpenCity 0.0.6 beta4
AppVersion=OpenCity 0.0.6 beta4
Compression=lzma
DefaultDirName={pf}\OpenCity
DefaultGroupName=OpenCity
LicenseFile=..\COPYING
SolidCompression=yes
VersionInfoVersion=0.0.6
;WindowVisible=no

[Languages]
Name: "de"; MessagesFile: "compiler:Languages\German.isl"
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "es"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "fr"; MessagesFile: "compiler:Languages\French.isl"
Name: "it"; MessagesFile: "compiler:Languages\Italian.isl"

[Files]
Source: "bin\Release\OpenCity-0.0.6beta.exe"; DestDir: "{app}\bin";
Source: "bin\Release\*.dll"; DestDir: "{app}\bin"
Source: "..\config\*"; DestDir: "{app}\etc\opencity\config"; Excludes: ".svn"
Source: "..\graphism\*"; DestDir: "{app}\share\opencity\graphism"; Excludes: ".svn"; Flags: recursesubdirs createallsubdirs
Source: "..\sound\*"; DestDir: "{app}\share\opencity\sound"; Excludes: ".svn"
Source: "docs\*.txt"; DestDir: "{app}\share\opencity\docs";
Source: "docs\FAQ.txt"; DestDir: "{app}"
Source: "docs\README.txt"; DestDir: "{app}"; Flags: isreadme
; MSVC C runtime manifest
;Source: "Release\Microsoft.VC80.CRT.manifest"; DestDir: "{app}\bin"

[Dirs]
Name: "{userappdata}\opencity\"

[Icons]
Name: "{group}\OpenCity 0.0.6beta - window"; Filename: "{app}\bin\OpenCity-0.0.6beta.exe"; IconFilename: "{app}\share\opencity\graphism\icon\OpenCity32.ico"; Parameters: "--data-dir ""{app}\share\opencity"" --conf-dir ""{app}\etc\opencity"""; WorkingDir: "{app}"
Name: "{group}\OpenCity 0.0.6beta - fullscreen (autodetect mode)"; Filename: "{app}\bin\OpenCity-0.0.6beta.exe"; IconFilename: "{app}\share\opencity\graphism\icon\OpenCity32.ico"; Parameters: "--data-dir ""{app}\share\opencity"" --conf-dir ""{app}\etc\opencity"" --full-screen"; WorkingDir: "{app}"
Name: "{group}\Configuration file"; Filename: "{app}\etc\opencity\config\opencity.xml"
Name: "{group}\Save folder"; Filename: "{userappdata}\opencity\"
Name: "{group}\Homepage"; Filename: "http://www.opencity.info"
Name: "{group}\Documentation\FAQ in English"; Filename: "{app}\FAQ.txt"
Name: "{group}\Documentation\FAQ in Italian"; Filename: "{app}\share\opencity\docs\FAQ_it.txt"
Name: "{group}\Documentation\INSTALL in Italian"; Filename: "{app}\share\opencity\docs\INSTALL_it.txt"
Name: "{group}\Documentation\README in English"; Filename: "{app}\README.txt";
Name: "{group}\Documentation\README in Italian"; Filename: "{app}\share\opencity\docs\README_it.txt"
Name: "{group}\Documentation\README in Spanish"; Filename: "{app}\share\opencity\docs\README_es.txt"
Name: "{group}\Uninstall"; Filename: "{uninstallexe}";

[UninstallDelete]
Type: filesandordirs; Name: "{app}"

[Run]
Filename: "{win}\explorer.exe"; Parameters: "http://www.opencity.info"; Flags: nowait skipifdoesntexist

[Code]
function InitializeSetup: Boolean;
var
  version: TWindowsVersion;
begin
  GetWindowsVersionEx(Version);

  // Print confirmation message for Windows after XP
  if (version.Major > 5) then
  begin
    if MsgBox('OpenCity may not work with your Windows installation, are you sure to continue ?',
      mbConfirmation, MB_YESNO) = IDNO then
      begin
        Result := False;
        Exit;
      end;
  end;

  Result := True;
end;
