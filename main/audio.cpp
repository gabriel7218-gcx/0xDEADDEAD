#include "audio.h"
#include "config.h"
#include "state.h"

#include <mmsystem.h>
#include <vector>

uint8_t bS(uint32_t t) {
    const uint32_t s =
        (uint32_t)BBSR * 30u;

    if (t < s) {
        return (uint8_t)(
            10 * t * (t >> 5 | t >> 9) & (t >> 2 | t >> 3)
        );
    }

    t -= s;

    return (uint8_t)(
        (uint32_t)(
            (t / 3.0) *
            ((t >> 4) | (t >> 8))
        ) & 255
    );
}

DWORD WINAPI aT(LPVOID) {
    const int sr = BBSR;
    const int cs = 5;
    const size_t bs = (size_t)sr * cs;
    const uint32_t lim = (uint32_t)sr * 30u;

    WAVEFORMATEX w = {};

    w.wFormatTag = WAVE_FORMAT_PCM;
    w.nChannels = 1;
    w.nSamplesPerSec = sr;
    w.wBitsPerSample = 8;
    w.nBlockAlign = 1;
    w.nAvgBytesPerSec = sr;

    HWAVEOUT o = NULL;

    if (
        waveOutOpen(
            &o,
            WAVE_MAPPER,
            &w,
            0,
            0,
            CALLBACK_NULL
        ) != MMSYSERR_NOERROR
    ) {
        return 0;
    }

    uint32_t t = 0;

    while (g_r) {
        std::vector<uint8_t> b(bs);

        for (size_t i = 0; i < bs; ++i) {
            if (t < lim) {
                b[i] = bS(t++);
            } else {
                b[i] = bS(lim + (t++ - lim));
            }
        }

        WAVEHDR h = {};

        h.lpData =
            reinterpret_cast<LPSTR>(b.data());

        h.dwBufferLength =
            (DWORD)b.size();

        if (
            waveOutPrepareHeader(
                o,
                &h,
                sizeof(h)
            ) != MMSYSERR_NOERROR
        ) {
            break;
        }

        if (
            waveOutWrite(
                o,
                &h,
                sizeof(h)
            ) != MMSYSERR_NOERROR
        ) {
            waveOutUnprepareHeader(
                o,
                &h,
                sizeof(h)
            );

            break;
        }

        while (!(h.dwFlags & WHDR_DONE)) {
            if (!g_r) {
                waveOutReset(o);
                break;
            }

            Sleep(20);
        }

        waveOutUnprepareHeader(
            o,
            &h,
            sizeof(h)
        );
    }

    waveOutReset(o);
    waveOutClose(o);

    return 0;
}