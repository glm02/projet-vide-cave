param(
    [string]$Xc8Version = 'v3.10',
    [string]$MplabVersion = 'v6.30',
    [string]$DfpVersion = '1.8.254'
)
$ErrorActionPreference = 'Stop'
$compilerPath = Join-Path $env:ProgramFiles "Microchip\xc8\$Xc8Version\bin\xc8-cc.exe"
$devicePackPath = Join-Path $env:ProgramFiles "Microchip\MPLABX\$MplabVersion\packs\Microchip\PIC12-16F1xxx_DFP\$DfpVersion\xc8"
if (!(Test-Path -LiteralPath $compilerPath)) { throw "XC8 introuvable : $compilerPath" }
if (!(Test-Path -LiteralPath $devicePackPath)) { throw "Pack PIC introuvable : $devicePackPath" }
$buildPath = Join-Path $PSScriptRoot 'build'
New-Item -ItemType Directory -Path $buildPath -Force | Out-Null
Push-Location -LiteralPath $buildPath
try {
    & $compilerPath '-mcpu=16F1788' "-mdfp=$devicePackPath" '-O0' '-o' 'test_led.elf' (Join-Path $PSScriptRoot 'main.c')
    if ($LASTEXITCODE -ne 0) { throw "Compilation echouee : code $LASTEXITCODE" }
    Write-Output "Compilation terminee. HEX : $(Join-Path $buildPath 'test_led.hex')"
} finally {
    Pop-Location
}
