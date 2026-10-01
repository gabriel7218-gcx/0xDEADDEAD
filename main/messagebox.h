#pragma once

#include <windows.h>

void rMB(
    int,
    int,
    int&,
    int&
);

LRESULT CALLBACK mH(
    int,
    WPARAM,
    LPARAM
);

int sMB();

DWORD WINAPI sMT(LPVOID);

DWORD WINAPI mBT(LPVOID);