#include "confirm.h"

#include <windows.h>

bool cf() {
    int r =
        MessageBoxW(
            NULL,
            L"This program will show some effects "
            L"on your desktop, play some sounds and crash.\n\n"
            L"Do you want to continue?",
            L"Warning",
            MB_YESNO |
            MB_ICONWARNING |
            MB_SYSTEMMODAL
        );

    if (r != IDYES)
        return false;

    r =
        MessageBoxW(
            NULL,
            L"LAST WARNING!\n\n"
            L"The creator of the program (Gabriel7218) is not "
            L"responsible for any damages to your machine.\n\n"
            L"You really want to start?",
            L"LAST WARNING",
            MB_YESNO |
            MB_ICONWARNING |
            MB_SYSTEMMODAL
        );

    return r == IDYES;
}