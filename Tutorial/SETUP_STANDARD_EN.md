# GrafiX — Standard Setup (English)

This guide shows the normal setup for building GrafiX with CLion on Windows.

## 1) Install Visual Studio (Build Tools)
1. Download **Visual Studio 2022 Community** from Microsoft:
   - https://visualstudio.microsoft.com/downloads/#:~:text=Free%20download,-Community
2. Run the installer.
3. Select the workload:
   - **Desktop development with C++**
4. Install.

> This gives you MSVC + the Windows SDK, which CLion needs.

## 2) Download / extract GrafiX
1. Download this repository as a ZIP.
   - Click **"Code"**, then **"Download ZIP"**
2. Extract it into your CLion projects folder, e.g.:
   - `C:\Users\<you>\CLionProjects\`
3. The folder is often named `GrafiX-main`. Rename it to:
   - `GrafiX`

## 3) Open in CLion and select toolchain
1. Open CLion → **Open** → select the `GrafiX` folder.
2. For Generator/Toolchain:
   - Generator: **Ninja**
   - Toolchain: **Visual Studio** (MSVC)

## 4) Configure (CMake) and Build
1. Let CMake configure (CLion usually does this automatically).
2. Select the **`GrafiX`** target and click **Build**.
3. For testing, build & run the demo target (**`GrafiX_Demo`**).

## 5) Consumer example (use GrafiX in another project)
1. Build GrafiX twice (**Debug** and **Release**).
2. In the new project:
   - Add this to your `CMakeLists.txt`:
     ```
     find_package(GrafiX REQUIRED PATHS "C:/Users/{YOUR_USERNAME}/CLionProjects/GrafiX")
     target_link_libraries(GFX_consumer_example PRIVATE GrafiX::GrafiX)
     grafix_copy_assets(GFX_consumer_example)
     ```
3. In `main.cpp`, add `#include <GrafiX/GrafiX.hpp>` at the very top.
4. Write the rest of your `main.cpp` and run it.
