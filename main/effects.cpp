#include "effects.h"
#include "config.h"
#include "state.h"

#include <cstdlib>
#include <cmath>
#include <cstdint>

static COLORREF rb(float h) {
    float c = 1.0f;

    float x =
        1.0f -
        fabsf(
            fmodf(
                h / 60.0f,
                2.0f
            ) - 1.0f
        );

    float r = 0;
    float g = 0;
    float b = 0;

    if (h < 60)
        r = c, g = x;
    else if (h < 120)
        r = x, g = c;
    else if (h < 180)
        g = c, b = x;
    else if (h < 240)
        g = x, b = c;
    else if (h < 300)
        r = x, b = c;
    else
        r = c, b = x;

    return RGB(
        (BYTE)(r * 255),
        (BYTE)(g * 255),
        (BYTE)(b * 255)
    );
}

static HBRUSH cRB(int t) {
    const int pw = 256;
    const int ph = 256;

    BITMAPINFO bi = {};

    bi.bmiHeader.biSize =
        sizeof(BITMAPINFOHEADER);

    bi.bmiHeader.biWidth = pw;
    bi.bmiHeader.biHeight = -ph;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;

    void* p = NULL;

    HDC d = GetDC(NULL);

    HBITMAP b =
        CreateDIBSection(
            d,
            &bi,
            DIB_RGB_COLORS,
            &p,
            NULL,
            0
        );

    ReleaseDC(NULL, d);

    if (!b || !p)
        return NULL;

    DWORD* px = (DWORD*)p;

    for (int y = 0; y < ph; ++y) {
        for (int x = 0; x < pw; ++x) {
            float h =
                fmodf(
                    t * 45.0f +
                    x * 1.4f +
                    y * 1.4f,
                    360.0f
                );

            px[y * pw + x] =
                rb(h);
        }
    }

    HBRUSH br =
        CreatePatternBrush(b);

    DeleteObject(b);

    return br;
}

static void mC(
    HDC ds,
    HDC dt,
    int x,
    int y,
    int w,
    int h,
    int a
) {
    if (
        w <= 0 ||
        h <= 0 ||
        a <= 0
    )
        return;

    if (a >= h)
        a = h - 1;

    BitBlt(
        dt,
        0,
        0,
        w,
        h,
        ds,
        x,
        y,
        SRCCOPY
    );

    BitBlt(
        ds,
        x,
        y + a,
        w,
        h - a,
        dt,
        0,
        0,
        SRCCOPY
    );

    PatBlt(
        ds,
        x,
        y,
        w,
        a,
        PATINVERT
    );
}

static void dRE(
    HDC d,
    const MI& m,
    DWORD e
) {
    int ix =
        GetSystemMetrics(SM_CXICON) / 2;

    int iy =
        GetSystemMetrics(SM_CYICON) / 2;

    int n =
        1 + (int)(e / 1000);

    if (n > 50)
        n = 50;

    int mx = m.w - ix;
    int my = m.h - iy;

    if (
        mx <= 0 ||
        my <= 0
    )
        return;

    for (int i = 0; i < n; ++i) {
        int x =
            m.x +
            rand() % mx;

        int y =
            m.y +
            rand() % my;

        DrawIcon(
            d,
            x,
            y,
            LoadIconA(
                NULL,
                IDI_ERROR
            )
        );
    }
}

void mFX(
    HDC d,
    DWORD e
) {
    if (g_m.empty())
        return;

    for (
        size_t i = 0;
        i < g_m.size();
        ++i
    ) {
        const MI& m = g_m[i];

        if (
            m.w <= 0 ||
            m.h <= 0
        )
            continue;

        int sx =
            (rand() % 21) - 10;

        int sy =
            (rand() % 21) - 10;

        BitBlt(
            d,
            sx,
            sy,
            m.w,
            m.h,
            d,
            m.x,
            m.y,
            SRCCOPY
        );

        HBRUSH b =
            cRB((int)(e / FDMS));

        if (b) {
            HBRUSH o =
                (HBRUSH)SelectObject(
                    d,
                    b
                );

            PatBlt(
                d,
                m.x,
                m.y,
                m.w,
                m.h,
                PATINVERT
            );

            SelectObject(
                d,
                o
            );

            DeleteObject(b);
        }

        for (int j = 0; j < 12; ++j) {
            int y =
                m.y +
                rand() % m.h;

            int h =
                1 +
                rand() % 20;

            if (y + h > m.y + m.h)
                h =
                    m.y + m.h - y;

            int o =
                (rand() % 101) - 50;

            BitBlt(
                d,
                m.x + o,
                y,
                m.w,
                h,
                d,
                m.x,
                y,
                SRCCOPY
            );
        }
    }

    for (
        size_t i = 0;
        i < g_m.size();
        ++i
    ) {
        dRE(
            d,
            g_m[i],
            e
        );
    }
}

void tFX(
    HDC ds,
    HDC dt,
    DWORD e
) {
    if (g_m.empty())
        return;

    for (
        size_t i = 0;
        i < g_m.size();
        ++i
    ) {
        const MI& m = g_m[i];

        if (
            m.w <= 0 ||
            m.h <= 0
        )
            continue;

        BitBlt(
            dt,
            0,
            0,
            m.w,
            m.h,
            ds,
            m.x,
            m.y,
            SRCCOPY
        );

        int n = 45 + (int)(e / 120);

        if (n > 90)
            n = 90;

        for (int j = 0; j < n; ++j) {
            int w;
            int h;
            int x;
            int y;
            int ox;
            int oy;

            int t = rand() % 100;

            if (t < 45) {
                w =
                    20 +
                    rand() % (m.w / 2 + 1);

                h =
                    2 +
                    rand() % 35;

                x =
                    rand() %
                    (m.w - w + 1);

                y =
                    rand() %
                    (m.h - h + 1);

                ox =
                    (rand() % 401) - 200;

                oy =
                    (rand() % 41) - 20;
            } else if (t < 80) {
                w =
                    40 +
                    rand() % (m.w / 2 + 1);

                h =
                    20 +
                    rand() % (m.h / 3 + 1);

                x =
                    rand() %
                    (m.w - w + 1);

                y =
                    rand() %
                    (m.h - h + 1);

                ox =
                    (rand() % 601) - 300;

                oy =
                    (rand() % 301) - 150;
            } else {
                w =
                    4 +
                    rand() % 80;

                h =
                    4 +
                    rand() % 80;

                if (w > m.w)
                    w = m.w;

                if (h > m.h)
                    h = m.h;

                x =
                    rand() %
                    (m.w - w + 1);

                y =
                    rand() %
                    (m.h - h + 1);

                ox =
                    (rand() % 501) - 250;

                oy =
                    (rand() % 501) - 250;
            }

            int sx = x + ox;
            int sy = y + oy;

            if (sx < 0)
                sx = 0;

            if (sy < 0)
                sy = 0;

            if (sx + w > m.w)
                sx = m.w - w;

            if (sy + h > m.h)
                sy = m.h - h;

            BitBlt(
                ds,
                m.x + x,
                m.y + y,
                w,
                h,
                dt,
                sx,
                sy,
                (rand() % 9 == 0)
                    ? NOTSRCCOPY
                    : SRCCOPY
            );
        }

        for (int j = 0; j < 8; ++j) {
            int y =
                rand() % m.h;

            int h =
                1 +
                rand() % 12;

            if (y + h > m.h)
                h = m.h - y;

            int ox =
                (rand() % 1001) - 500;

            int sx =
                ox;

            if (sx < 0)
                sx = 0;

            if (sx + m.w > m.w)
                sx = 0;

            BitBlt(
                ds,
                m.x,
                m.y + y,
                m.w,
                h,
                dt,
                sx,
                y,
                SRCCOPY
            );
        }

        if ((rand() % 7) == 0) {
            PatBlt(
                ds,
                m.x,
                m.y,
                m.w,
                m.h,
                PATINVERT
            );
        }
    }
}