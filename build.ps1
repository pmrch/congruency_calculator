using namespace System.Collections.Generic

#$PSStyle.OutputRendering = 'ANSI'

[string] $cStandard = "clatest"
#[string] $CC = "clang-cl.exe"
[string] $CC = "cl.exe"
[string] $LNK = "link.exe"
[string] $cTarget = "congruency-calculator"

#[string] $cOptimization = "/Gy /fp:fast /clang:-O3 /clang:-march=native /clang:-DNDEBUG"
[string] $cOptimization = "/Gy /fp:fast /O2 /arch:AVX512"

#[string] $cDebug = "-fsanitize=address /Oy- -fno-omit-frame-pointer"

#[string] $mimallocLib = "$env:VCPKG_ROOT\installed\x64-windows-static\lib"
[string] $cWinVer = "/D_WIN32_WINNT=0x0A00"
[string] $cIncludes = "/Iinclude"

[string] $cStrictFlags = @("/clang:-Weverything", "/clang:-Werror", "/clang:-Wno-unsafe-buffer-usage")
#[string] $cCflags = "/nologo /std:$cStandard -fcolor-diagnostics -fansi-escape-codes /MT $cOptimization $cIncludes $cStrictFlags $cWinVer"
[string] $cCflags = "/nologo /std:$cStandard /MT $cOptimization $cIncludes $cWinVer"
[string] $cLdflags = "/nologo /SUBSYSTEM:CONSOLE"

#[string] $cCflags = "/nologo /std:$cStandard fcolor-diagnostics -fansi-escape-codes /MT $cDebug $cIncludes $cStrictFlags $cWinVer"
#[string] $cLdflags = "/nologo -fsanitize=address /link /SUBSYSTEM:CONSOLE"

[string] $ccJson = "compile_commands.json"
Set-Content -Path $ccJson -Value "[]" -Encoding UTF8

[string] $cCwd = $PWD.Path
[string] $cSrc = Join-Path $cCwd "src"

[System.IO.DirectoryInfo] $cBuild = [System.IO.Path]::Combine($cCwd, "build")
if (-not $cBuild.Exists) { New-Item -ItemType Directory -Path $cBuild | Out-Null }

class ObjectFile {
    [string] $fullPath
    [string] $fileName

    ObjectFile([string] $fPath, [string] $fName) {
        $this.fullPath = $fPath
        $this.fileName = $fName
    }
}


[List[ObjectFile]] $objFileList = [List[ObjectFile]]::new()

function Compile {
    param (
        [Parameter(Mandatory)]
        [int]$LogLevel
    )
    
    [List[PSCustomObject]] $compileDb = [List[PSCustomObject]]::new()
    [string[]] $baseArgs = ($cCflags -split '\s+')
    if (-not (([System.IO.DirectoryInfo]$ccJson).Exists)) { Write-Output "" > $ccJson }

    Get-ChildItem -Path $cSrc -Filter "*.c" -Recurse -File | ForEach-Object {
        #[string] $relativePath = [System.IO.Path]::GetRelativePath($cSrc, $_.FullName)
	[string] $fName = $_.FullName
	[string] $relativePath = "$cSrc\$fName"

        [string] $objName = ($relativePath -replace '[\\/]', '_') -replace '\.c$', '.obj'
        [string] $objPath = [System.IO.Path]::Combine($cBuild, $objName)
        [string] $srcFileFullPath = $_.FullName

        #[string[]] $extraArgs = @("/clang:-DLOG_LEVEL=$LogLevel", "/c", "$srcFileFullPath", "/Fo:$objPath")
	[string[]] $extraArgs = @("/c", "$srcFileFullPath", "/Fo:$objPath")
        [string[]] $cArgs = $baseArgs + $extraArgs

        [PSCustomObject] $dbEntry = [ordered]@{ 
            arguments = @($CC) + ($cArgs | ForEach-Object { $_ -replace '\\', '/' }) 
            directory = $cCwd -replace '\\', '/' 
            file      = $srcFileFullPath -replace '\\', '/' 
            output    = $objPath -replace '\\', '/'
        }
        
        $compileDb.Add($dbEntry)
        
        [string]$relPathNorm = $relativePath -replace '\\', '/'
        Write-Host "src/$relPathNorm => build/$objName"
        # Write-Host "Command: $CC $cArgs"
        $objFileList.Add([ObjectFile]::new($objPath, $objName))

        $outString = (& $CC $cArgs 2>&1) -join [System.Environment]::NewLine
        [int] $exitCode = $LASTEXITCODE

        if (-not ($exitCode -eq 0)) { 
            Write-Host "$outString" 
            Break
        }
    }

    if ($compileDb.Count -gt 0) {
        [string] $jsonContent = $compileDb.ToArray() | ConvertTo-Json -Depth 10
        Set-Content -Path $ccJson -Value $jsonContent -Encoding UTF8
    }

    $LASTEXITCODE = 0
}

function LinkObjs {
    param ()
    
    if (-not ($objFileList.Count -eq 0)) {
        [string[]] $objPaths = foreach ($obj in $objFileList) { $obj.fullPath }
        [string[]] $linkArgs = $objPaths + "/Fe:$cTarget.exe" + ($cLdflags -split '\s+')

        [string] $outString = (& $LNK $linkArgs 2>&1) -join [System.Environment]::NewLine
        [int] $exitCode = $LASTEXITCODE

        # Write-Host "Linking: $CC $linkArgs"
        Write-Host "Linking executable... " -NoNewline
        if (-not ($exitCode -eq 0)) {
            Write-Host "FAIL"    
            Write-Host "Failed to link final executable: $outString"
            Return
        }

        Write-Host "OK"
    }
}

[int] $logLvl = if ($args.Count -gt 0) { $args[0] } else { 1 }
Compile($logLvl)
LinkObjs