@echo off

set BUILD_TOOL="C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe"
%BUILD_TOOL% insolence.sln /v:n /tv:14.0 /p:Configuration=Debug

IF NOT "%ERRORLEVEL%" == "0" (
	goto :EOF
)

%BUILD_TOOL% insolence_samples.sln /v:quiet /tv:14.0 /p:Configuration=Debug

IF NOT "%ERRORLEVEL%" == "0" (
	goto :EOF
)

cmd.exe /c MKLINK /J "bin\assets" "src\insolence\assets"
cmd.exe /c MKLINK /J "bin\shaders" "src\insolence\shaders"

cd bin
insolence_samples.exe
