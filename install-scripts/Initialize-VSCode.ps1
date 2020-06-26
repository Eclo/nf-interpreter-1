# Copyright (c) 2020 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.

# This PS configures Visual Studio Code to use with nanoFramework:
# sets/updates the required settings in settings.json 
# sets/updates the launch.json from the targets
# sets/updates the cmake-variants.json from the targets

function Main {
    param (
        [Parameter(HelpMessage = "Please enter the COM port for ESP32 flash utility [e.g. COM1].")]
        [string]$COMPort,
        [Parameter(HelpMessage = "Updates the settings.json file from the template.")]
        [switch]$UpdateSettings = $false,
        [Parameter(HelpMessage = "Updates the launch.json configuration for the various targets from the templates on the targets directory.")]
        [switch]$UpdateLaunch = $false,
        [Parameter(HelpMessage = "Force setting/updating the values even if they already exist.")]
        [switch]$force = $false
        )
    
    Import-Module -Name "$PSScriptRoot\configure-vscode-common.psm1"

    # arguments to be passed to the called scripts
    $commandArgs = ""

    If ([string]::IsNullOrEmpty($COMPort)) {
        "IF you're planning to use ESP32, please use parameter '-COMPort' to set COM port for ESP32 flash utility [e.g. -COMPort COM1], or edit tasks.json and cmake-variants.json" | Write-Host -ForegroundColor Yellow
    }
    else {
        $env:NF_ESP32_COMPORT = $COMPort

        $commandArgs = " -COMPort $COMPort"
    }

    ############################
    # settings.json

    # path to settings.json file
    $filePath = Join-Path -Path $(Get-VSCodeFolderPath) -ChildPath "settings.json"

    # flag for file changes
    $fileChanges = $false

    # file exists
    $fileExists = Test-Path $filePath -ErrorAction SilentlyContinue

    if ($fileExists -ne $true) {
        # create file
        Set-Content -Path "$filePath" -Value '{ }' -Encoding UTF8
    }

    # read settings file as json
    $settings = Get-Content -Raw -Encoding UTF8 $filePath | ConvertFrom-Json

    if ($null -ne $settings) {

        # check 'cmake.configureSettings' 
        if ($null -eq $settings."cmake.configureSettings") {
            # create 'cmake.configureSettings'
            $settings | Add-Member -MemberType NoteProperty -Name 'cmake.configureSettings' -Value $(New-Object PSObject)
        
            # flag changes
            $fileChanges = $true
        }

        # set CMAKE_MAKE_PROGRAM with Ninja in case it's not available on the PATH inside "cmake.configureSettings"
        if ($null -eq $settings."cmake.configureSettings".CMAKE_MAKE_PROGRAM -or $force) {
            # need to replace forward slash for paths to work with GCC and CMake
            $ninjaPath = Join-Path -Path $env:NINJA_PATH -ChildPath 'ninja.exe'
            $ninjaPath = $ninjaPath.Replace('\', '/') +

            $settings."cmake.configureSettings" | Add-Member -MemberType NoteProperty -Name 'CMAKE_MAKE_PROGRAM' -Value "$ninjaPath"
        
            # flag changes
            $fileChanges = $true
        }

        # setting Ninja as the preferred generator
        if ($null -eq $settings."cmake.generator" -or $force) {
            $settings | Add-Member -MemberType NoteProperty -Name 'cmake.generator' -Value 'Ninja'
        
            # flag changes
            $fileChanges = $true
        }

        # save changes, if needed
        if ($fileChanges) {

            $settings | ConvertTo-Json -depth 32 | Out-File $filePath -Encoding UTF8 -Force

            CleanJsonPatterns($filePath)
        }
    }

    # need to pass the 'force' switch?
    if ($force) {
        $commandArgs = $commandArgs + " -force"
    }

    # set/update launch.json
    . .\Set-VSCode-Launch.ps1
    Set-VSCode-Launch $commandArgs

    # set/update tasks.json
    . .\Set-VSCode-Tasks.ps1
    Set-VSCode-Tasks $commandArgs
    
    # set/update cmake-variants.json
    . .\Set-VSCode-CMakes.ps1
    Set-VSCode-CMakes $commandArgs
}

Main
