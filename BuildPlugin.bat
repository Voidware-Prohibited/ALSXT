@echo off

set EngineVersion=5.4

FOR %%A IN ("%~dp0.") DO SET ParentFolder=%%~dpA
SET AlsFolder=%ParentFolder%ALS-Refactored

if not exist %AlsFolder% (
  echo ALS-Refactored not found in Plugins directory. Clone or Download ALS-Refactored into the Plugins folder to proceed.
) else (
  echo ALS-Refactored directory found.
  for /F %%i in ('dir /b /a "%AlsFolder%*"') do (
    echo ALS-Refactored directory not empty. Starting Build.
    for /f "skip=2 tokens=2*" %%a in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\EpicGames\Unreal Engine\%EngineVersion%" /v "InstalledDirectory"') do set "EngineDirectory=%%b"

    set AutomationToolPath="%EngineDirectory%\Engine\Build\BatchFiles\RunUAT.bat"
    set PluginPath="%cd%\ALSXT.uplugin"
    set OutputPath="%cd%\Build"

    title Build Plugin
    echo Automation Tool Path: %AutomationToolPath%
    echo:

    call %AutomationToolPath% BuildPlugin -Plugin=%PluginPath% -Package=%OutputPath% -Rocket -TargetPlatforms=Win64
    echo:
    pause
    exit 0
  )
)
