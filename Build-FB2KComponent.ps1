<#
.SYNOPSIS
    Builds the foobar2000 component package.
.DESCRIPTION
    This script will be executed unconditionally during the Post-build step. It copies all the necessary files to an output directory and creates the zip archive.
.EXAMPLE
    C:\PS> .\Build-FB2KComponent.ps1
.OUTPUTS
    *.fb2k-component
#>

[CmdletBinding()]
param
(
    [parameter(Mandatory, HelpMessage='Component Name')]
        [string] $ComponentName,
    [parameter(Mandatory, HelpMessage='Component File Name')]
        [string] $SourceFileName,
    [parameter(Mandatory, HelpMessage='Platform')]
        [string] $Platform,
    [parameter(Mandatory, HelpMessage='Source Path')]
        [string] $SourcePath
)

#Requires -Version 7.2

Set-StrictMode -Version Latest;
Set-PSDebug -Strict; # Equivalent of VBA "Option Explicit".

$ErrorActionPreference = 'Stop';

# Note: The working directory is the solution directory.

Write-Host "Building package `"$ComponentName`" ($Platform)...";

$OutPath     = '..\out';
$PackagePath = "$OutPath\$ComponentName";

# Create the package directory (including the x64 subdirectory)
Write-Host "Creating directory `"$PackagePath`"...";

$null = New-Item -Path $OutPath -Name "$ComponentName\x64" -ItemType 'directory' -Force;
$null = New-Item -Path $OutPath -Name "$ComponentName\ARM64EC" -ItemType 'directory' -Force;

if ($Platform -eq 'x64')
{
    if (Test-Path -Path "$SourcePath\$SourceFileName")
    {
        Write-Host "Copying $SourceFileName to `"$PackagePath\$Platform`"...";

        Copy-Item "$SourcePath\$SourceFileName" -Destination "$PackagePath\$Platform" -Force -Verbose;
    }

    # Install the component in the foobar2000 x64 components directory.
    $foobar2000Path = '..\bin';

    if (Test-Path -Path "$foobar2000Path\foobar2000.exe")
    {
        $ComponentPath = "$foobar2000Path\profile\user-components-x64";

        Write-Host "Creating directory `"$ComponentPath\$ComponentName`"...";

        $null = New-Item -Path "$ComponentPath" -Name "$ComponentName" -ItemType 'directory' -Force;

        Write-Host "Installing x64 component in foobar2000 64-bit profile...";

        Copy-Item "$PackagePath\x64\*.dll"  -Destination "$ComponentPath\$ComponentName" -Force -Verbose;
    }
    else
    {
        Write-Host "Skipped component installation: foobar2000 64-bit directory not found.";
    }
}
elseif ($Platform -eq 'Win32')
{
    if (Test-Path -Path "$SourcePath\$SourceFileName")
    {
        Write-Host "Copying $SourceFileName to `"$PackagePath`"...";

        Copy-Item "$SourcePath\$SourceFileName" -Destination "$PackagePath" -Force -Verbose;
    }

    # Install the component in the foobar2000 x86 components directory.
    $foobar2000Path = '..\bin.x86';

    if (Test-Path -Path "$foobar2000Path\foobar2000.exe")
    {
        $ComponentPath = "$foobar2000Path\profile\user-components";

        Write-Host "Creating directory `"$ComponentPath\$ComponentName`"...";

        $null = New-Item -Path "$ComponentPath" -Name "$ComponentName" -ItemType 'directory' -Force;

        Write-Host "Installing x86 component in foobar2000 32-bit profile...";

        Copy-Item "$PackagePath\*.dll"  -Destination "$ComponentPath\$ComponentName" -Force -Verbose;
    }
    else
    {
        Write-Host "Skipped component installation: foobar2000 32-bit directory not found.";
    }
}
elseif ($Platform -eq 'ARM64EC')
{
    if (Test-Path -Path "$SourcePath\$SourceFileName")
    {
        Write-Host "Copying $SourceFileName to `"$PackagePath\$Platform`"...";

        Copy-Item "$SourcePath\$SourceFileName" -Destination "$PackagePath\$Platform" -Force -Verbose;
    }
    else
    {
        Write-Host "Skipped component installation: foobar2000 32-bit directory not found.";
    }
}
else
{
    Write-Host "Unknown platform: $Platform";
    exit;
}

$ArchivePath = "$OutPath\$ComponentName.fb2k-component";

Write-Host "Creating component archive `"$ArchivePath`"...";

Compress-Archive -Force -Path $OutPath\$ComponentName\* -DestinationPath $ArchivePath;

Write-Host "Done.";
