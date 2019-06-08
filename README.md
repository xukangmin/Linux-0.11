# Linux-0.11

The old Linux kernel source ver 0.11 which has been tested under modern Linux, macOS and Windows.

## Windows 10 Setup

* Upgrade Windows 10 to version 1903(19H1) or latter.
* Enable WSL ([Windows subsystem of Linux](https://docs.microsoft.com/windows/wsl)), download a Linux distro from Microsoft store or Github. install it.
* Install VSCode and install **Remote Development** extension.
* install make, gcc, gdb and binutils in **wsl**
* install qemu, bochs in **windows**
* Open new wsl window in vscode (see [docs for Remote-wsl](https://aka.ms/vscode-remote/wsl/getting-started)), install C/C++ extension on **wsl**

### Access file in wsl

* You can access wsl's Linux files in Explorer(or any win32 app) at `\\wsl$\Ubuntu` (or you distro name)
* You can access Windows's files in wsl at `/mnt/c` (or other drive letter)

### Known issue

* You can't mount minix image file in wsl1. wait for release of wsl2 or use a real Linux.

## macOS Setup

* install Xcode command line tools: `xcode-select --install`
* install [Homebrew](https://brew.sh/)
* install i386-elf cross compiler and toolchain: gcc, gdb and binutils
* install qemu, bochs
* install VSCode
* install VSCode C/C++ extension
* configure C/C++ extension

```bash
brew install qemu bochs i386-elf-binutils i386-elf-gcc i386-elf-gdb
# i386-elf-gdb have conflict file with i386-elf-binutils : /usr/local/share/info/bfd.info
brew link --overwrite i386-elf-gdb
```

```diff
--- a/.vscode/launch.json
+++ b/.vscode/launch.json
@@ -11,7 +11,8 @@
             "cwd": "${workspaceFolder}",
             "environment": [],
             "MIMode": "gdb",
-            "miDebuggerPath": "gdb",
+            "miDebuggerPath": "i386-elf-gdb",
             "targetArchitecture": "x86",
             "setupCommands": [
                 {
                     "description": "Enable pretty-printing for gdb",
--- a/.vscode/c_cpp_properties.json
+++ b/.vscode/c_cpp_properties.json
@@ -6,7 +6,7 @@
                 "${workspaceFolder}/**"
             ],
             "defines": [],
-            "compilerPath": "/usr/bin/gcc",
+            "compilerPath": "/usr/local/bin/i386-elf-gcc",
             "cStandard": "c89",
             "cppStandard": "c++98",
             "intelliSenseMode": "gcc-x64"
```

### Limit in macOS

* You can't mount minix image file in macOS. It doesn't support minix filesystem.

### setup vscode

## modern Linux Setup

* install gcc, gdb, binutils, qemu, bochs and VSCode
* install VSCode C/C++ extension

## Build and run

```bash
# build
make
# run in qemu
make run
# debug in bochs
make bochs
# open debug server from qemu
make debug
# open gdb to connect with qemu
make gdb
# clean files
make clean
# mount hdc-0.11.img to hdc (only on linux and WSL2)
make mount
# umount hdc
make umount
```

### debug kernel in vscode

first run qemu in debug mode

```bash
make debug
```

Press F5 to launch vscode debug
