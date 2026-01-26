#pragma once

struct ma_engine;
struct ma_sound;

class Sound;

class SoundEngine {
public:
	~SoundEngine() = delete;
	SoundEngine() = delete;

	SoundEngine(const SoundEngine&) = delete;
	SoundEngine(const SoundEngine&&) = delete;
	SoundEngine& operator=(const SoundEngine&) = delete;
	SoundEngine& operator=(const SoundEngine&&) = delete;

	static int init_audio();
	static void uninit_audio();

	static void set_position(float x, float y, float z);
	static void set_direction(float x, float y, float z);
	static void set_volume(float volume);

	static float get_volume();

	static bool get_OK() { return is_OK; }

	static ma_engine* get_engine();

private:
	static bool is_OK;

	static ma_engine* m_soundEngine;
};
