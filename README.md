## 🛠️ Setup Instructions

### **1️⃣ Install Dependencies**

Ensure you have the following installed on your system:

- **C++ Compiler** (GCC for Linux/macOS, MinGW for Windows)
- **CMake** (Required for building)
- **Filesystem support** (C++17 or later required)

#### **Install CMake & GCC (Linux/macOS)**

```sh
sudo apt update && sudo apt install build-essential cmake
```

#### **Install MinGW & CMake (Windows)**

1. Install [MinGW-w64](https://www.mingw-w64.org/)
2. Install [CMake](https://cmake.org/download/)
3. Add both to the system `PATH`

---

## 🔧 **Build & Run Instructions**

### **For Linux/macOS** 🐧🍏

```sh
chmod +x run.sh
./run.sh
```

### **For Windows** 🖥️

```powershell
./run.ps1
```

If you face execution permission issues on Linux/macOS, run:

```sh
chmod +x run.sh
```

---

## 🛠️ **Troubleshooting**

### **Error: `LibraryEntry() is ambiguous`**

- Ensure all constructors are correctly defined in `LibraryEntry`.
- Check if you have default constructors for missing parameters.

### **Error: `Error opening accounts.json`**

- Ensure the `data/` folder exists.
- The program should create missing files automatically.

