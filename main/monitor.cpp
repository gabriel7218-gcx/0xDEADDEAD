#include "monitor.h"
#include "state.h"

BOOL CALLBACK eM(
    HMONITOR,
    HDC,
    LPRECT r,
    LPARAM
) {
    MI m = {};

    m.x = r->left;
    m.y = r->top;
    m.w = r->right - r->left;
    m.h = r->bottom - r->top;

    g_m.push_back(m);

    return TRUE;
}