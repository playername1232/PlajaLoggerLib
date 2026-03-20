# PlajaLogger

Simple cross-platform C++ logging library with support for Windows (.dll/.lib) and Linux (.so).

---

## 📦 Features

* Timestamped logs
* Log levels: `DEBUG`, `INFO`, `WARNING`, `ERROR`
* Key-value appends
* Cross-platform (Windows + Linux)
* Easy integration into other C++ projects

---

## 🛠️ Prerequisites

### Windows (BuildAndExport.ps1)

You need:

* **CMake** (>= 3.20)
* **Visual Studio Build Tools 2022** *(with C++ workload)*

#### Install (PowerShell):

```powershell
winget install Kitware.CMake
winget install Microsoft.VisualStudio.2022.BuildTools
```

👉 During installation of Build Tools, make sure:

* ✔️ "Desktop development with C++" is selected

#### Verify:

```powershell
cmake --version
cl
```

---

### Linux (BuildAndExport.sh)

You need:

* **g++**
* **make**
* **cmake**

#### Install (Ubuntu / WSL):

```bash
sudo apt update
sudo apt install build-essential cmake
```

#### Verify:

```bash
g++ --version
cmake --version
```

---

## 🚀 Build & Export (Library)

### Linux

```bash
chmod +x BuildAndExport.sh
./BuildAndExport.sh
```

Output:

```
build/LinuxExport/
├── PlajaLogger.h
└── libplaja_logger.so
```

---

### Windows

```powershell
.\BuildAndExport.ps1
```

Output:

```
build/WindowsExport/
├── PlajaLogger.h
├── plaja_logger.dll
└── plaja_logger.lib
```

---

## 🧪 Build Test Project

### Linux

```bash
chmod +x BuildAndExport.sh
./BuildAndExport.sh
```

Output:

```
Builds/LinuxBuild/
├── test_vlastnich_lib
└── libplaja_logger.so
```

Run:

```bash
./Builds/LinuxBuild/test_vlastnich_lib
```

---

### Windows

```powershell
.\BuildAndExport.ps1
```

Output:

```
Builds/WindowsBuild/
├── test_vlastnich_lib.exe
├── plaja_logger.dll
└── plaja_logger.lib
```

Run:

```powershell
.\Builds\WindowsBuild\test_vlastnich_lib.exe
```

---

## 📌 Usage Example

```cpp
#include "PlajaLogger.h"

int main()
{
    PlajaLogger::Debug("User created")
        .AppendValue("id", 42)
        .AppendValue("name", "Martin")
        .Write();

    return 0;
}
```

Output:

```
[19.03.2026 22:54] [DEBUG] User created: id=42 | name=Martin
```

---

## ⚠️ Notes

* `.dll` / `.so` must be present next to the executable at runtime
* `.lib` (Windows) is required only for linking
* Header (`.h`) is always required

---

## 🧠 Dev Notes

* Templates are implemented in header (`AppendValue`)
* Export macro handles cross-platform compatibility
* Built using CMake

---

## ❤️ Author

Martin Krepčík

#ChatGpt README a nestydím se za to :)