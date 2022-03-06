
# ---This script sets up the build environment for nmake


# $env:Path += ";C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.15.26726\bin\HostX86\x86" 
# $env:Path += ";C:\Program Files\GIMP 2\32\bin"
# $env:Path += ";C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\CommonExtensions\Microsoft\TeamFoundation\Team Explorer\Git\mingw32\bin"

$env:Path =  "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\CommonExtensions\Microsoft\TeamFoundation\Team Explorer\Git\mingw32\bin;$env:path"
$env:Path = "C:\Program Files\GIMP 2\32\bin;$env:path"
$env:Path = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.15.26726\bin\HostX86\x86;$env:path"
