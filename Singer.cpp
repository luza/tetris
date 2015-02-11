#include <vector>
#include "SDL.h"
#include "Singer.h"

const char *wavFilename[Singer::MAX_SOUND] = {
    "resources/click16.wav",    // SOUND_CLICK
    "resources/drop16.wav",        // SOUND_DROP
    "resources/cleared16.wav",    // SOUND_WHOOP
    "resources/levelup16.wav",    // SOUND_GONG
    "resources/game_over16.wav"    // SOUND_ORCHESTRA
};

std::vector<Singer::buffer> Singer::m_buffersList;

void SDLCALL
Singer::fillerup(void *unused, Uint8 *stream, int len)
{
    // silence
    SDL_memset(stream, 0, len);

    std::vector<buffer>::iterator i;
    for (i=m_buffersList.begin(); i!=m_buffersList.end(); /*i++*/) {
        struct buffer buf = *i;
        Uint32 left = buf.len - (buf.ptr - buf.start);

        if (left < (Uint32)len) {
            len = left;
        }

        // mix in the buffer content
        SDL_MixAudio(stream, buf.ptr, len, SDL_MIX_MAXVOLUME);
        buf.ptr += len;

        if (buf.ptr >= buf.start + buf.len) {
            // buffer is over
            i = m_buffersList.erase(i);
            continue;
        }

        *i = buf;
        ++i;
    }

    // if no buffers left, trigger pause
    if (m_buffersList.empty()) {
        SDL_PauseAudio(1);
    }
}

Singer::Singer()
{
    m_mute = false;

    SDL_AudioSpec wav_spec;

    for (int i=0; i<MAX_SOUND; i++) {
        SDL_LoadWAV(
            wavFilename[i],
            &wav_spec,
            &m_sounds[i].start,
            &m_sounds[i].len
        );
        m_sounds[i].ptr = m_sounds[i].start;
    }

    wav_spec.callback = &Singer::fillerup;

    /* Initialize fillerup() variables */
    if (SDL_OpenAudio(&wav_spec, NULL) < 0) {
        return;
    }
}

Singer::~Singer()
{
    SDL_CloseAudio();

    // free the sounds
    for (int i=0; i<MAX_SOUND; i++) {
        SDL_FreeWAV(m_sounds[i].start);
    }
}

void
Singer::play(t_sound sound)
{
    if (m_mute) {
        return;
    }

    buffer buf = m_sounds[sound];
    m_buffersList.push_back(buf);
    SDL_PauseAudio(0);
}

void
Singer::setMute(bool mute)
{
    m_mute = mute;
}
