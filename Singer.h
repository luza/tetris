#pragma once

#include <vector>

#define BUFFER_LEN		1024*8	// 8 KB

class Singer
{
public:
	Singer();
	~Singer();
	static void SDLCALL fillerup(void *unused, Uint8 *stream, int len);

	enum t_sound {
		SOUND_CLICK,
		SOUND_DROP,
		SOUND_WHOOP,
		SOUND_GONG,
		SOUND_ORCHESTRA,
		MAX_SOUND
	};

	void play(t_sound sound);
	void setMute(bool mute);

private:
	struct buffer {
		Uint8 *start;
		Uint8 *ptr;
		Uint32 len;
	};

	buffer m_sounds[MAX_SOUND];
	bool m_mute;

	static std::vector<buffer> m_buffersList;
};
