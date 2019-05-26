# Linux-0.11

The old Linux kernel source ver 0.11 which has been tested under modern Linux,  Mac OSX and Windows.

## Build on macOS

### macOS Setup

* install Homebrew
* install i386-elf cross compiler gcc and binutils
* install qemu, bochs
* install gdb
* a linux-0.11 hardware image file: hdc-0.11.img (already in this repo)

```bash
brew install qemu bochs i386-elf-binutils i386-elf-gcc i386-elf-gdb
# i386-elf-gdb have conflict file with i386-elf-binutils : /usr/local/share/info/bfd.info
brew link --overwrite i386-elf-gdb
```

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
```

### debug in vscode

first run qemu in debug mode

```bash
make debug
```

Press F5 to launch vscode debug