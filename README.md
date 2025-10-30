
# Butler

Butler is a shopping list like basic home accounting tool.

## Installation

### Ubuntu 24.04 (Noble)

Download and install the latest Debian package from the [Releases](https://github.com/csjpeter/butler/releases) page:

```bash
# Replace with the actual version you downloaded
sudo dpkg -i butler0.1_*-24.04-noble_amd64.deb
```

If you encounter dependency issues, run:

```bash
sudo apt-get install -f
```

After installation, you can run Butler with:

```bash
butler0.1
```

Or find it in your application menu as "Butler".

### Building from Source

#### Requirements

- Qt5 (5.15 or later recommended)
- SQLite3
- CMake 3.14 or later
- C++17 compiler

#### Build with CMake

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

The binary will be located at `build/obj/butler` (relative to the build directory).

To run:
```bash
./build/obj/butler
```

#### Build with qmake

```bash
qmake
make
```

#### Build Debian Package

To build the Debian package yourself:

```bash
sudo apt-get install debhelper-compat cmake pkg-config qtbase5-dev qtbase5-dev-tools libqt5sql5-sqlite libsqlite3-dev dpkg-dev build-essential
dpkg-buildpackage -us -uc -b
```

The `.deb` packages will be created in the parent directory.

## Execute

After building from source with CMake:

```bash
cd build
./build/obj/butler
```

Or with qmake:
```bash
./butler
```

## Development

Built with Qt5 (requires Qt5.15 or later recommended).

### Project Structure

- `src/` - Main source code
- `libcsjp/` - Bundled library code
- `share/` - Icons and desktop files
- `debian/` - Debian packaging files
- `.github/workflows/` - CI/CD workflows

## License

See LICENSE file for details.

