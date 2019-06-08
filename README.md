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

## modern Linux Setup

* install gcc, gdb, binutils, qemu, bochs and VSCode

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
