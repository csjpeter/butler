#!/usr/bin/env bash
# Installs development dependencies for building Butler on Ubuntu 24.x
# Run as a regular user; sudo will be used for apt operations.
set -euo pipefail

PKGS=(
  build-essential
  cmake
  pkg-config
  git
  ninja-build
  ccache

  qtbase5-dev
  qttools5-dev-tools
  qtbase5-dev-tools

  libsqlite3-dev
  libssl-dev
  libicu-dev

  libqt5svg5-dev
  libqt5websockets5-dev

  valgrind
  gdb
  lcov

  ca-certificates
  apt-transport-https
  software-properties-common

  debhelper
  dh-make
  fakeroot
  lintian
)

echo "Updating package lists..."
sudo apt-get update

echo "Installing packages: ${PKGS[*]}"
sudo apt-get install -y --no-install-recommends "${PKGS[@]}"

echo "Removing unused packages and cleaning apt cache..."
sudo apt-get autoremove -y
sudo apt-get clean

cat <<'EOF'
Done.
Next steps:
  - Make the script executable: chmod +x install_ubuntu24_devel_dependencies.sh
  - Create a build directory: mkdir -p build && cd build
  - Configure and build: cmake .. -DCMAKE_BUILD_TYPE=Debug && cmake --build .
EOF
