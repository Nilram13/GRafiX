# GrafiX — Standard Setup (Deutsch)

Diese Anleitung zeigt das normale Setup zum Bauen von GrafiX in CLion unter Windows.

## 1) Visual Studio (Build Tools) installieren
1. Lade **Visual Studio 2022 Community** von Microsoft:
   - https://visualstudio.microsoft.com/de/downloads/#:~:text=Mitwirkende%20und%20Einzelpersonen-,Kostenloser%20Download,-Professional
2. Starte den Installer.
3. Wähle die Workload:
   - **Desktopentwicklung mit C++**
4. Installieren.

> Damit bekommst du MSVC + Windows SDK, die CLion braucht.

## 2) GrafiX herunterladen / entzippen
1. Lade dieses Repo als ZIP herunter.
   - Klick auf "Code", dann auf "download ZIP"
3. Entzippe es in deinen CLion-Projekte-Ordner, z.B.:
   - `C:\Users\<du>\CLionProjects\`
4. Der Ordner heißt oft `GrafiX-main`. Benenne ihn um in:
   - `GrafiX`

## 3) In CLion öffnen und Toolchain wählen
1. CLion öffnen → **Open** → `GrafiX` Ordner auswählen.
2. Bei Generator/Toolchain:
   - Generator: **Ninja**
   - Toolchain: **Visual Studio** (MSVC)

## 4) Konfigurieren (CMake) und Build
1. CMake durchlaufen lassen (macht CLion normalerweise automatisch).
2. Target **`GrafiX`** auswählen und **Build** klicken.
3. Zum Testen: Demo-Target bauen & starten (`GrafiX_Demo`).

## 5) Consumer Beispiel (GrafiX in anderem Projekt nutzen)
1. GrafiX 2 mal bauen (Debug und Release).
2. Im neuen Projekt:
   - In `CMakeLists` diesen Teil hinzufügen:
     ```
     find_package(GrafiX REQUIRED PATHS "C:/Users/{YOUR_USERNAME}/CLionProjects/GrafiX")
     target_link_libraries(GFX_consumer_example PRIVATE GrafiX::GrafiX)
     grafix_copy_assets(GFX_consumer_example)
     ```
3. In `main.cpp` `#include <GrafiX/GrafiX.hpp>` ganz oben hinzufügen.
4. Restlichen Code für `main.cpp` schreiben und ausführen.
