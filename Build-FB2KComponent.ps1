<#
.SYNOPSIS
    Builds the foobar2000 component package.
.DESCRIPTION
    This script will be executed unconditionally during the Post-build step. It copies all the necessary files to an output directory and creates the zip archive.
.EXAMPLE
    C:\PS> .\Build-FB2KComponent.ps1
.OUTPUTS
    foo_midi.fb2k-component
#>

[CmdletBinding()]
param
(
    [parameter(Mandatory, HelpMessage='Target Name')]
        [string] $TargetName,
    [parameter(Mandatory, HelpMessage='Target File Name')]
        [string] $TargetFileName,
    [parameter(Mandatory, HelpMessage='Platform')]
        [string] $Platform,
    [parameter(Mandatory, HelpMessage='OutputPath')]
        [string] $OutputPath
)

#Requires -Version 7.2

Set-StrictMode -Version Latest;
Set-PSDebug -Strict; # Equivalent of VBA "Option Explicit".

$ErrorActionPreference = 'Stop';

# Note: The working directory is the solution directory.

Write-Host "Building package `"$TargetName`"...";

if ($Platform -eq 'x64')
{
    $PackagePath = "../out/$TargetName/x64";

    if (!(Test-Path -Path $PackagePath))
    {
        Write-Host "Creating directory `"$PackagePath`"...";
        $null = New-Item -Path '../out/' -Name "$TargetName/x64" -ItemType 'directory';
    }

    if (Test-Path -Path "$OutputPath/$TargetFileName")
    {
        Write-Host "Copying $TargetFileName to `"$PackagePath`"...";
        Copy-Item "$OutputPath/$TargetFileName" -Destination "$PackagePath";
    }

    if (Test-Path -Path "../bin")
    {
        Write-Host "Installing component in foobar2000 32-bit...";
        Copy-Item "$PackagePath/../*" "../bin/x86/profile/user-components/$TargetName" -Force;

        Write-Host "Installing component in foobar2000 64-bit...";
        Copy-Item "$PackagePath/*" "../bin/profile/user-components-x64/$TargetName" -Force;
    }
}
elseif ($Platform -eq 'Win32')
{
    $PackagePath = "../out/$TargetName";

    if (!(Test-Path -Path $PackagePath))
    {
        Write-Host "Creating directory `"$PackagePath`"...";
        $null = New-Item -Path '../out/' -Name "$TargetName/x64" -ItemType 'directory';
    }

    if (Test-Path -Path "$OutputPath/$TargetFileName")
    {
        Write-Host "Copying $TargetFileName to `"$PackagePath`"...";
        Copy-Item "$OutputPath/$TargetFileName" -Destination "$PackagePath";
    }

    if (Test-Path -Path "../bin")
    {
        Write-Host "Installing component in foobar2000 32-bit...";
        Copy-Item "$PackagePath/*" "../bin/x86/profile/user-components/$TargetName" -Force;

        Write-Host "Installing component in foobar2000 64-bit...";
        Copy-Item "$PackagePath/x64/*" "../bin/profile/user-components-x64/$TargetName" -Force;
    }
}
else
{
    Write-Host "Unknown platform: $Platform";
    exit;
}

$PackagePath = "../out/$TargetName";

Compress-Archive -Force -Path ../out/$TargetName/* -DestinationPath ../out/$TargetName.fb2k-component;

Write-Host "Done.";
