# 0xDEADDEAD

**My first GDI prank program.**

A small Windows prank program written in C++ using the Win32 API and GDI.

## Features

* GDI visual effects
* Rainbow / screen effects
* Randomized visual behavior
* Message boxes
* Sound effects
* Multiple threads
* A few questionable decisions

## Building

Requires a Windows-compatible MinGW-w64 toolchain.

Example:

```bash
i686-w64-mingw32-g++ main.cpp state.cpp monitor.cpp audio.cpp messagebox.cpp effects.cpp confirm.cpp crash.cpp -o 0xDEADDEAD.exe -lgdi32 -luser32 -lwinmm -mwindows
```

## Compatibility

Designed primarily for Windows systems.

Tested on:

* Windows XP
* Windows 7 (32-bit & 64-bit)
* Windows 10 (64-bit)

Some effects may behave differently depending on the Windows version and hardware.

## Warning

This program is intended as a prank/experimental graphics project.

**Don't run it on a computer you don't control.**
