@echo off
pushd %~dp0\..\
call cmake -B Build/Projects -G "Visual Studio 17 2022"
popd

PAUSE
