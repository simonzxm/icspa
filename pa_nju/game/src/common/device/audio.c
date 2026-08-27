#include "device/audio.h"
#include "common.h"
#include "x86.h"

void audio_pause(int cmd) {
    out_longbyte(AUDIO_CTL, cmd);
}

void audio_write(Audio_Data *audio) {
    out_longbyte(AUDIO_DATA, (int32_t)audio);
}
