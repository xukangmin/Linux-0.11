# Linux-0.11

The old Linux kernel source ver 0.11 which has been tested under modern Linux,  Mac OSX and Windows.

# modern Linux Setup

* install gcc, gdb and binutils
* install qemu, bochs
* a linux-0.11 hardware image file: hdc-0.11.img (already in this repo)

### Build and run

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