if ($args[0] -eq "--linux" -or $args[0] -eq "-l") {
    .\clean.ps1
    wsl.exe ./build.sh
    exit 0
}

if (-not (Test-Path build/)) {
    mkdir build
}
Set-Location build
cmake ../
$vs = (Get-CimInstance MSFT_VSInstance).InstallLocation
$msbuild = "MSBuild\Current\Bin\MSBuild.exe"
& "$vs\$msbuild" jpp_test.sln
Set-Location ../