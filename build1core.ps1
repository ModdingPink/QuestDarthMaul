param($p1)
$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt

$buildScript = "$NDKPath/build/ndk-build"

$OS = $PSVersionTable.OS
if ($OS)
{
    $windows = $OS.Contains("Windows")
}
else
{
    $windows = 'True'
}

# when core, or when on windows we want to add .cmd to the end
if ((-not ($PSVersionTable.PSEdition -eq "Core")) -or $windows) {
    $buildScript += ".cmd"
}

if ($p1 -eq "clean")
{
    echo "Removing obj dir for clean build"
    remove-item ./obj -Recurse
}

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk
