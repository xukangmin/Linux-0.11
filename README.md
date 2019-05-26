# Linux-0.11

The old Linux kernel source ver 0.11 which has been tested under modern Linux,  macOS and Windows.

`master` branch is config and instructions for Windows10. see Linux or macOS instructions in branch `linux` or `macos`

## Windows 10 Setup

* Enable WSL ([Windows subsystem of Linux](https://docs.microsoft.com/windows/wsl)), download a Linux distro from Microsoft store or Github. install it.
* Install VSCode **Insider** (use vscode insider until vscode stable May 2019 update release in June 2019),and install **Remote Development**.
* install make, gcc, gdb and binutils in **wsl**
* install qemu, bochs in **windows**
* a linux-0.11 hardware image file: hdc-0.11.img (already in this repo)
* Open new wsl window in vscode (see [docs for Remote-wsl](https://aka.ms/vscode-remote/wsl/getting-started)), install C/C++ extension on **wsl**

## Access file in wsl

* You can access wsl's Linux files in Explorer(or any win32 app) at `\\wsl$\Ubuntu` (or you distro name) (need Windows 10 update 1903)
* You can access Windows's files in wsl at `/mnt/c` (or other drive letter)

### Build and run

Run all commands in wsl

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
```

### debug in vscode

first run qemu in debug mode

```bash
make debug
```

Press F5 to launch vscode debug

# Known issue

* You can't mount minix image file in wsl1. wait for release of wsl2 or use a real Linux.