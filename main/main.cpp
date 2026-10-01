#include <windows.h>
#include <cstdlib>
#include <ctime>

#include "config.h"
#include "state.h"
#include "monitor.h"
#include "audio.h"
#include "messagebox.h"
#include "effects.h"
#include "confirm.h"
#include "crash.h"

int WINAPI WinMain(
    HINSTANCE,
    HINSTANCE,
    LPSTR,
    int
) {
    if (!cf())
        return 0;

    srand(
        (unsigned)time(NULL)
    );

    EnumDisplayMonitors(
        NULL,
        NULL,
        eM,
        0
    );

    if (g_m.empty())
        return 1;

    int mw = 0;
    int mh = 0;

    for (
        size_t i = 0;
        i < g_m.size();
        ++i
    ) {
        if (g_m[i].w > mw)
            mw = g_m[i].w;

        if (g_m[i].h > mh)
            mh = g_m[i].h;
    }

    HDC ds =
        GetDC(NULL);

    if (!ds)
        return 1;

    HDC dt =
        CreateCompatibleDC(ds);

    if (!dt) {
        ReleaseDC(
            NULL,
            ds
        );

        return 1;
    }

    HBITMAP bm =
        CreateCompatibleBitmap(
            ds,
            mw,
            mh
        );

    if (!bm) {
        DeleteDC(dt);

        ReleaseDC(
            NULL,
            ds
        );

        return 1;
    }

    SelectObject(
        dt,
        bm
    );

    HANDLE at =
        CreateThread(
            NULL,
            0,
            aT,
            NULL,
            0,
            NULL
        );

    HANDLE mt =
        CreateThread(
            NULL,
            4096,
            mBT,
            NULL,
            0,
            NULL
        );

    if (mt)
        CloseHandle(mt);

    DWORD st =
        GetTickCount();

    while (g_r) {
        if (
            GetAsyncKeyState(VK_LEFT) &
            0x8000
        ) {
            g_r = false;
            break;
        }

        DWORD e =
            GetTickCount() -
            st;

        if (e < 30000) {
            mFX(
                ds,
                e
            );
        } else {
            tFX(
                ds,
                dt,
                e - 30000
            );
        }

        if (
            e >= 60000 &&
            !g_fPS
        ) {
            g_fPS = true;
            kWI();
        }

        if (
            GetAsyncKeyState(VK_LEFT) &
            0x8000
        ) {
            g_r = false;
            break;
        }

        Sleep(FDMS);
    }

    g_r = false;

    if (at) {
        WaitForSingleObject(
            at,
            2000
        );

        CloseHandle(at);
    }

    DeleteObject(bm);
    DeleteDC(dt);

    ReleaseDC(
        NULL,
        ds
    );

    return 0;
}