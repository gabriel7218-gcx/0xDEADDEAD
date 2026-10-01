#include "messagebox.h"
#include "state.h"

#include <cstdlib>

void rMB(
    int w,
    int h,
    int& x,
    int& y
) {
    if (g_m.empty()) {
        x = CW_USEDEFAULT;
        y = CW_USEDEFAULT;
        return;
    }

    const MI& m =
        g_m[rand() % g_m.size()];

    int mx = m.w - w;
    int my = m.h - h;

    if (mx < 0)
        mx = 0;

    if (my < 0)
        my = 0;

    static LONG c = 0;

    LONG n =
        InterlockedIncrement(&c);

    int nc = 5;
    int nr = 4;

    int cx = n % nc;
    int cy = (n / nc) % nr;

    int cw = mx / nc;
    int ch = my / nr;

    x =
        m.x +
        cx * cw +
        (
            cw > w
                ? rand() % (cw - w + 1)
                : 0
        );

    y =
        m.y +
        cy * ch +
        (
            ch > h
                ? rand() % (ch - h + 1)
                : 0
        );
}

LRESULT CALLBACK mH(
    int c,
    WPARAM wp,
    LPARAM lp
) {
    if (c == HCBT_ACTIVATE) {
        HWND h = (HWND)wp;
        RECT r = {};

        if (GetWindowRect(h, &r)) {
            int w = r.right - r.left;
            int ht = r.bottom - r.top;

            int x;
            int y;

            rMB(
                w,
                ht,
                x,
                y
            );

            SetWindowPos(
                h,
                HWND_TOP,
                x,
                y,
                0,
                0,
                SWP_NOSIZE |
                SWP_NOACTIVATE |
                SWP_SHOWWINDOW
            );
        }
    }

    return CallNextHookEx(
        NULL,
        c,
        wp,
        lp
    );
}

int sMB() {
    HHOOK h =
        SetWindowsHookExW(
            WH_CBT,
            mH,
            NULL,
            GetCurrentThreadId()
        );

    int r =
        MessageBoxW(
            NULL,
            L"Try closing me!!",
            L"Try closing me!!",
            MB_OK |
            MB_ICONERROR |
            MB_SYSTEMMODAL
        );

    if (h)
        UnhookWindowsHookEx(h);

    return r;
}

DWORD WINAPI sMT(LPVOID) {
    if (g_r)
        sMB();

    return 0;
}

DWORD WINAPI mBT(LPVOID) {
    while (g_r) {
        int n = (int)g_m.size();

        if (n <= 0)
            break;

        for (
            int i = 0;
            i < 20 && g_r;
            ++i
        ) {
            int mi = i % n;

            HANDLE h =
                CreateThread(
                    NULL,
                    4096,
                    sMT,
                    (LPVOID)(INT_PTR)mi,
                    0,
                    NULL
                );

            if (h)
                CloseHandle(h);

            Sleep(10);
        }

        Sleep(10000);
    }

    return 0;
}