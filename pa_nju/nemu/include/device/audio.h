#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "device/port_io.h"
#include "nemu.h"

#include <SDL/SDL.h>

#define AUDIO_DATA 0xC0
#define AUDIO_CTL 0xC1
#define AUDIO_IRQ 2

make_pio_handler(audio_io_handler);

void audio_start();
void audio_stop();

#endif
