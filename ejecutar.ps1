param(
    [string]$CarpetaDatos = 'data',
    [string]$Compilador = '',
    [string]$Prolog = ''
)

$ErrorActionPreference = 'Stop'

# Ayuda de ejecucion en Windows; los tres modulos siguen en C++, Haskell y Prolog.
if (-not $Compilador) {
    $comandoCpp = Get-Command g++ -ErrorAction SilentlyContinue
    if ($comandoCpp) { $Compilador = $comandoCpp.Source }
    elseif (Test-Path -LiteralPath 'C:\msys64\mingw64\bin\g++.exe') {
        $Compilador = 'C:\msys64\mingw64\bin\g++.exe'
    }
    else { throw 'No se encontro g++. Indique su ruta con -Compilador.' }
}

if (-not $Prolog) { $Prolog = $env:SWIPL }
if (-not $Prolog) {
    $comandoProlog = Get-Command swipl -ErrorAction SilentlyContinue
    if ($comandoProlog) { $Prolog = $comandoProlog.Source }
    else {
        $candidatosProlog = @(
            (Join-Path $env:ProgramFiles 'swipl\bin\swipl.exe'),
            (Join-Path $env:USERPROFILE '.cache\transporte-prolog\portable\bin\swipl.exe')
        )
        foreach ($candidato in $candidatosProlog) {
            if (Test-Path -LiteralPath $candidato) { $Prolog = $candidato; break }
        }
    }
}

$rutaAnterior = $env:PATH
$prologAnterior = $env:SWIPL
Push-Location -LiteralPath $PSScriptRoot
try {
    $env:PATH = (Split-Path -Parent $Compilador) + ';' + $env:PATH
    if ($Prolog) {
        $env:SWIPL = $Prolog
        $env:PATH = (Split-Path -Parent $Prolog) + ';' + $env:PATH
    }
    else { Write-Host 'La opcion 9 requiere SWI-Prolog. Puede indicar su ruta con -Prolog.' }
    New-Item -ItemType Directory -Force -Path build | Out-Null
    & $Compilador -std=c++11 cpp/main.cpp cpp/Registro/registro.cpp cpp/Transbordos/transbordos.cpp cpp/data/structures.cpp cpp/data/persistencia.cpp -o build/transporte.exe
    if ($LASTEXITCODE -ne 0) { throw 'La compilacion de C++ fallo.' }
    & .\build\transporte.exe $CarpetaDatos
    if ($LASTEXITCODE -ne 0) { throw 'La ejecucion del sistema termino con un error.' }
}
finally {
    $env:PATH = $rutaAnterior
    $env:SWIPL = $prologAnterior
    Pop-Location
}
