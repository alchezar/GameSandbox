:: Copyright © 2024, Ivan Kinder
@echo off

:: Set the variables.
set FoldersToRemove=.vs Binaries DerivedDataCache Intermediate Saved
set FilesToRemove=GameSandbox.sln

:: Remove intermediate folders.
for %%f in (%FoldersToRemove%) do (
	rmdir /s /q ..\%%f
)

:: Remove solution file.
for %%f in (%FilesToRemove%) do (
	del /q ..\%%f
)