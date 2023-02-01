:: @file Run.cmd @author Joel Barrett @date 01/01/12 @brief Executes the application.

@echo off

pushd "%~dp0\Application\Main\Bin\Release"
start "" "Main.exe"

popd & exit /b 0
