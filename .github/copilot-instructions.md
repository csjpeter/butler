# Copilot Instructions for Butler

## Project Overview

Butler is a shopping list and basic home accounting tool built with Qt5 and C++17. It provides a desktop application for managing shopping lists, tracking purchases, and basic financial accounting for home use.

**Key Technologies:**
- C++17
- Qt5 (5.15 or later recommended)
- SQLite3 for data storage
- CMake build system

## Project Structure

```
butler/
├── src/                      # Main application code
│   ├── qt/                   # Qt-specific UI and application code
│   └── test/                 # Application data classes tests
├── libcsjp/                  # Bundled library code
│   ├── container/            # Container data structures (arrays, trees, JSON)
│   ├── core/                 # Core utilities (file, string, mutex)
│   ├── devel/                # Development utilities (exception, logger)
│   ├── system/               # System/network (socket, epoll, HTTP, WebSocket)
│   └── human/                # Human interface utilities (requires ICU)
├── share/                    # Icons and desktop files
├── debian/                   # Debian packaging files
└── .github/workflows/        # CI/CD workflows
```

## Development Setup

### Prerequisites

Install the required dependencies:

**Ubuntu/Debian:**
```bash
sudo apt-get install qtbase5-dev libsqlite3-dev cmake build-essential
```

### Building the Project

**CMake (Recommended):**
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

The binary will be located at `build/obj/butler` (relative to the build directory).

**qmake (Alternative):**
```bash
qmake
make
```

### Running Tests

Tests require a `testdir` directory in the project root:

```bash
mkdir -p testdir
cd build
ctest --build-config Debug --output-on-failure
```

## Code Organization and Architecture

### Naming Conventions

- **Classes**: PascalCase (e.g., `ItemQueryStat`, `PaymentQueryStat`)
- **Files**: snake_case with prefix (e.g., `butler_dataclasses.cpp`, `csjp_string.cpp`)
- **Methods/Functions**: camelCase
- **Constants/Macros**: UPPER_SNAKE_CASE

### libcsjp Library

The project includes a custom library (`libcsjp`) with reusable components:

- **Container**: Custom container types (arrays, trees, JSON)
- **Core**: String utilities, file I/O, mutex implementations
- **Devel**: Exception handling, logging, utility functions
- **System**: Network programming (sockets, HTTP, WebSocket, epoll)

All libcsjp code uses the `csjp` namespace. Application code typically includes `using namespace csjp;`.

### Qt Architecture

Butler uses Qt5 Widgets for the UI:

- Model classes inherit from Qt's model/view framework
- Custom views extend Qt widget classes
- Main window architecture with multiple view components
- Data models for tables and lists

## Code Style Guidelines

### General Style

- Use C++17 features where appropriate
- Follow existing indentation and formatting in the file you're editing
- Include copyright headers matching existing files:
  ```cpp
  /**
   * Author: Csaszar, Peter <csjpeter@gmail.com>
   * Copyright (C) YEAR Csaszar, Peter
   */
  ```

### Headers and Includes

- Use include guards: `#ifndef PROJECT_MODULE_H` / `#define PROJECT_MODULE_H`
- Include order: system headers, Qt headers, libcsjp headers, butler headers
- Use `<>` for library includes, `""` for local includes where applicable

### Memory Management

- Prefer RAII and smart pointers
- Qt's parent-child relationship manages memory for Qt objects
- Use libcsjp containers (`OwnerContainer`, `RefArray`, etc.) appropriately

## Testing Requirements

### Test Organization

Tests are organized by module:
- `libcsjp/container/test/` - Container data structure tests
- `libcsjp/core/test/` - Core utilities tests
- `libcsjp/devel/test/` - Development utilities tests
- `libcsjp/system/test/` - System/network tests
- `src/test/` - Application data classes tests
- `src/qt/test/` - Qt-specific code tests

### Running Tests

All tests are run through CMake's CTest:
```bash
mkdir -p testdir  # Required by some tests
cd build
ctest --build-config Debug --output-on-failure
```

### Writing New Tests

- Follow existing test patterns in the relevant test directory
- Each test module has a `CMakeLists.txt` that registers the test executable
- Tests should be self-contained and not depend on external state
- Use descriptive test names

## Build and CI/CD

### Continuous Integration

Two GitHub Actions workflows are configured:

1. **Build Workflow** (`.github/workflows/build-workflow.yml`)
   - Runs on: pushes to `dev/*` branches, PRs to `master`/`stable`/`release/*`
   - Builds on: Ubuntu (gcc), Windows (MSVC)
   - Configuration: Debug build
   - Runs full test suite with CTest

2. **Debian Package Workflow** (`.github/workflows/debian-package.yml`)
   - Runs on: version tags (`v*`), PRs to `master`/`stable`/`release/*`
   - Builds `.deb` packages for Ubuntu 24.04
   - Creates GitHub releases for tagged versions

### Compiler Flags

The project uses specific compiler flags (see `CMakeLists.txt`):
- `-Wno-expansion-to-defined`
- `-Wno-deprecated-copy`
- `-Wno-implicit-fallthrough`
- `-Wno-unused-function`

Debug builds include: `-Og -g3 -ggdb3`

## Common Pitfalls and Tips

### Build Issues

1. **Qt version mismatch**: Ensure Qt5.15+ is installed. Older versions may have compatibility issues.
2. **Missing testdir**: Some tests require a `testdir` directory in the project root. Create it before running tests.
3. **SQLite dependency**: Make sure `libsqlite3-dev` is installed on Linux systems.

### Code Changes

1. **Modifying libcsjp**: Changes to `libcsjp` affect both the library tests and the main application. Run both test suites.
2. **Qt MOC**: Header files with `Q_OBJECT` macro need MOC processing. CMake handles this automatically with `CMAKE_AUTOMOC ON`.
3. **String types**: The project uses custom string types from libcsjp. Familiarize yourself with `csjp::String` and `csjp::AStr`.

### Testing

1. **Test failures on Windows**: Some tests may be Unix-specific. Check test conditions.
2. **File system tests**: Tests in `libcsjp/core/test/file.cpp` may require specific permissions or directories.
3. **Network tests**: Tests in `libcsjp/system/test/` may require network access or specific ports.

## Platform-Specific Considerations

### Linux/Ubuntu
- Primary development platform
- Debian packages are built for Ubuntu 24.04
- Use `dpkg-buildpackage` for creating distribution packages

### Windows
- Qt installation via Choco: `choco install qt5-default`
- Some libcsjp features may have platform-specific implementations (see `csjp_utility_*.cpp`)

### Android
- Android support files exist in `android/` directory
- Requires Android-specific build configuration

## Deployment

### Debian Package

To build a Debian package:
```bash
sudo apt-get install debhelper-compat cmake pkg-config qtbase5-dev \
                     qtbase5-dev-tools libqt5sql5-sqlite libsqlite3-dev \
                     dpkg-dev build-essential
dpkg-buildpackage -us -uc -b
```

Packages are created in the parent directory.

### Installation

After building the package:
```bash
sudo dpkg -i butler0.1_*-24.04-noble_amd64.deb
sudo apt-get install -f  # If dependency issues occur
```

Run the application:
```bash
butler0.1
```

## Contributing

When making changes:

1. Create a feature branch from `dev/*` or appropriate branch
2. Make minimal, focused changes
3. Ensure all tests pass locally before pushing
4. The Build Workflow will run on pushes to `dev/*` branches
5. Pull requests to `master`, `stable`, or `release/*` trigger full CI

## Commit Message Guidelines

Keep commit messages compact and focused:

- **Maximum 5 lines total** including the subject line
- Subject line: Brief summary of changes (50 characters or less)
- Body (if needed): 2-3 lines maximum explaining the what and why
- Use imperative mood ("Add feature" not "Added feature")
- No need for extensive details - code changes speak for themselves

## Additional Resources

- Main repository: https://github.com/csjpeter/butler
- Qt5 Documentation: https://doc.qt.io/qt-5/
- CMake Documentation: https://cmake.org/documentation/
