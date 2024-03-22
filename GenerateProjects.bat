@echo off

if "%VULKAN_SDK%" == "" (
    echo No valid Vulkan SDK installation found. Please install Vulkan before proceeding.
    PAUSE
) else (
    call vendor\premake\premake5.exe vs2022
    PAUSE
)