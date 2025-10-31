
# Butler

Butler is a shopping list like basic home accounting tool.

## Installation

### Windows

Download and run the latest Windows installer from the [Releases](https://github.com/csjpeter/butler/releases) page.

The installer will:
- Install Butler to `C:\Program Files\butler`
- Create Start Menu shortcuts
- Create a Desktop shortcut
- Register the application in Windows Add/Remove Programs

After installation, you can run Butler from the Start Menu or Desktop shortcut.

### Ubuntu 24.04 (Noble)

Download and install the latest Debian package from the [Releases](https://github.com/csjpeter/butler/releases) page:

```bash
# Replace with the actual version you downloaded
sudo dpkg -i butler_*_amd64.deb
```

If you encounter dependency issues, run:

```bash
sudo apt-get install -f
```

After installation, you can run Butler with:

```bash
butler
```

Or find it in your application menu as "Butler".

#### Available Packages

The project provides three Debian packages:

- **butler**: The main package with optimized release binary
- **butler-dbg**: Debug symbols for the release binary (for debugging optimized code)
- **butler-debug**: Non-optimized debug build with full debugging symbols (for development and troubleshooting)

To install the debug version for development:

```bash
sudo dpkg -i butler-debug_*_amd64.deb
butler-debug
```

### Building from Source

#### Requirements

- Qt5 (5.15 or later recommended)
- SQLite3
- CMake 3.14 or later
- C++17 compiler

#### Linux (Ubuntu/Debian)

##### Build with CMake

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

The binary will be located at `build/build/obj/butler`.

To run:
```bash
./build/build/obj/butler
```

##### Build with qmake

```bash
qmake
make
```

##### Build Debian Package

To build the Debian package yourself:

```bash
sudo apt-get install debhelper-compat cmake pkg-config qtbase5-dev qtbase5-dev-tools libqt5sql5-sqlite libsqlite3-dev dpkg-dev build-essential
dpkg-buildpackage -us -uc -b
```

The `.deb` packages will be created in the parent directory.

#### Windows

##### Prerequisites

- Visual Studio 2019 or later with C++ development tools
- CMake 3.14 or later
- Qt5 5.15.2 or later
- SQLite3

##### Build with CMake

Using vcpkg to install dependencies (recommended):

```powershell
# Install vcpkg if not already installed
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Install dependencies
.\vcpkg install sqlite3:x64-windows

# Install Qt5 using aqtinstall (or install manually from qt.io)
pip install aqtinstall
aqt install-qt windows desktop 5.15.2 win64_msvc2019_64

# Build Butler
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="<path-to-qt>\5.15.2\msvc2019_64;<path-to-vcpkg>\installed\x64-windows"
cmake --build . --config Release
```

The executable will be located at `build\build\obj\Release\butler.exe`.

To create an installer, you'll also need NSIS (Nullsoft Scriptable Install System):

```powershell
choco install nsis
# Then follow the steps in .github/workflows/windows-installer.yml
```

## Execute

### Linux

After building from source with CMake:

```bash
cd build
./build/obj/butler
```

Or with qmake:
```bash
./butler
```

### Windows

After building from source with CMake:

```powershell
cd build
.\build\obj\Release\butler.exe
```

Or run from the installation directory if you installed using the installer.

## Development

Built with Qt5 (requires Qt5.15 or later recommended).

### Project Structure

- `src/` - Main source code
- `libcsjp/` - Bundled library code
- `share/` - Icons and desktop files
- `debian/` - Debian packaging files
- `.github/workflows/` - CI/CD workflows

### Testing

The project includes unit tests for the libcsjp library and application code.

#### Running Tests

To run all tests:

```bash
mkdir -p testdir  # Some tests require this directory
cd build
ctest --build-config Debug --output-on-failure
```

#### Test Organization

- `libcsjp/container/test/` - Container data structure tests
- `libcsjp/core/test/` - Core utilities (file, string, mutex) tests
- `libcsjp/devel/test/` - Development utilities (exception) tests
- `libcsjp/system/test/` - System/network (socket, epoll, HTTP) tests
- `src/test/` - Application data classes tests
- `src/qt/test/` - Qt-specific code tests

Tests are automatically run by the Build Workflow on pull requests and pushes to development branches.

## License

See LICENSE file for details.


