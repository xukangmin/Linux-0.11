# Linux-0.11

The old Linux kernel source ver 0.11 which has been tested under modern Linux, macOS and Windows.

## modern Linux Setup

* Install gcc, gdb, binutils, qemu, bochs and VSCode
* Install VSCode C/C++ extension

## Windows 10 Setup

* Upgrade Windows 10 to version 1903(19H1) or latter.
* Enable WSL ([Windows subsystem of Linux](https://docs.microsoft.com/windows/wsl)), download a Linux distro from Microsoft store or Github. install it.
* Install VSCode and install **Remote Development** extension.
* Install make, gcc, gdb and binutils in **wsl**
* Install qemu, bochs in **windows**
* Open new wsl window in vscode (see [docs for Remote-wsl](https://aka.ms/vscode-remote/wsl/getting-started)), install C/C++ extension on **wsl**
* Add all tools to `PATH`, so you can excute them directly from wsl shell.
* Run all command in wsl shell.(eg. `make`)

### Access file in wsl

* You can access wsl's Linux files in Explorer(or any win32 app) at `\\wsl$\Ubuntu` (or you distro name)
* You can access Windows's files in wsl at `/mnt/c` (or other drive letter)

### Known issue

* You can't mount minix image file in wsl1. wait for release of wsl2 or use a real Linux.

## Windows Setup (not recommend)

This may not work.Use it only when you can't use WSL.

* install `msys2`
* install `make` in msys2 shell: `pacman -S make`
* [download](https://github.com/lordmilko/i686-elf-tools/releases) prebuilt GNU `i686-elf` toolchain for Windows
* install qemu, bochs
* install VSCode
* install VSCode C/C++ extension
* Add all tools to `PATH`, so you can excute them directly from msys2 shell.
* modify VSCode C/C++ configuration in `.vscode`,set proper path for gcc and gdb.
* Run all command in msys2 shell.(eg. `make`)

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

Apply this diff.

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
