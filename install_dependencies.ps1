# vcpkg is a C/C++ package manager (docs & install https://vcpkg.io/en/getting-started.html)
# Current Directory should be vcpkg when running script.
# Requires -RunAsAdministrator

echo "Installing needed dependencies for CLEO v5 build process"
./vcpkg integrate install
./vcpkg install curl:x86-windows-static
./vcpkg install cpr:x86-windows-static
./vcpkg install jsoncpp:x86-windows-static
echo "Finished installing dependencies"
