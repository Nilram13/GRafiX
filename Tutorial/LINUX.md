# GrafiX — Linux Setup

Follow the standard setup guide for windows, with the following adjustments for Linux:

## 1. System Dependencies
Install the required development packages for graphics, windowing, and audio before opening CLion:

- **Ubuntu / Debian:**
  ```bash
  sudo apt update && sudo apt install -y build-essential cmake ninja-build libx11-dev libxrandr-dev libxcursor-dev libudev-dev libgl1-mesa-dev libopenal-dev libfreetype6-dev
  ```

- **Arch:**
  ```bash
  sudo pacman -S --needed base-devel cmake ninja xorg-server-devel mesa freetype2 openal
  ```


## 2. CLion Toolchain

Skip step 1 of the Windows guide (Visual Studio is not needed).
In CLion (Settings → Build, Execution, Deployment → Toolchains), select GCC or Clang (default on Linux) instead of Visual Studio / MSVC.


## 3. Consumer Projects

Use relative paths or ${CMAKE_CURRENT_SOURCE_DIR} instead of C:/Users/... so your project works cross-platform:

```CMake
find_package(GrafiX REQUIRED PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../GrafiX")
```
