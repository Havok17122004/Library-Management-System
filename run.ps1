if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}
Set-Location -Path "build"
cmake ..
cmake --build .
Start-Process -NoNewWindow -FilePath ".\library_system.exe"
