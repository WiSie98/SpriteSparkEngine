#include "Sparkpch.h"

#define DR_WAV_IMPLEMENTATION
#include "External/dr_wav.h"

#define DR_FLAC_IMPLEMENTATION
#include "External/dr_flac.h"

#define DR_MP3_IMPLEMENTATION
#include "External/dr_mp3.h"

#include "SparkCore/HeaderFiles/Sound.h"

namespace SpriteSpark {

    Sound::Sound() : m_Buffer(0), m_Source(0), m_Device(nullptr), m_Context(nullptr), m_IsLooping(false), m_IsBufferLoaded(false) {
        SP_CORE_TRACE("Initializing OpenAL");
        initOpenAL();
        SP_CORE_TRACE("OpenAL initializatized successfully!");
    }

    Sound::Sound(const std::string& filepath) : m_Buffer(0), m_Source(0), m_Device(nullptr), m_Context(nullptr), m_IsLooping(false), m_IsBufferLoaded(false) {
        SP_CORE_TRACE("Initializing OpenAL");
        initOpenAL();
        SP_CORE_TRACE("Loading audio file");

        if (loadSound(filepath)) {
            SP_CORE_INFO("Audio loaded successfully from: ", filepath);
            m_IsBufferLoaded = true;
        } else {
            std::cerr << "Failed to load audio file: " << filepath << std::endl;
        }

        SP_CORE_TRACE("Initialization complete!");
    }

    Sound::Sound(const char* filepath) : Sound(std::string(filepath)) {}

    Sound::~Sound() {
        cleanupOpenAL();
        SP_CORE_INFO("OpenAL closed successfully!");
    }

    void Sound::initOpenAL() {
        m_Device = alcOpenDevice(NULL);
        if (!m_Device) {
            std::cerr << "Failed to open sound device" << std::endl;
            return;
        }

        m_Context = alcCreateContext(m_Device, NULL);
        if (!m_Context || alcMakeContextCurrent(m_Context) == ALC_FALSE) {
            if (m_Context) alcDestroyContext(m_Context);
            alcCloseDevice(m_Device);
            std::cerr << "Failed to create or make context current" << std::endl;
            return;
        }

        alGenBuffers(1, &m_Buffer);
        alGenSources(1, &m_Source);
    }

    void Sound::cleanupOpenAL() {
        alDeleteSources(1, &m_Source);
        alDeleteBuffers(1, &m_Buffer);
        alcMakeContextCurrent(NULL);
        if (m_Context) alcDestroyContext(m_Context);
        if (m_Device) alcCloseDevice(m_Device);
    }

    bool Sound::loadWAV(const std::string& filepath) {
        drwav wav;
        if (!drwav_init_file(&wav, filepath.c_str(), NULL)) {
            return false;
        }

        std::vector<drwav_int16> pcmData(wav.totalPCMFrameCount * wav.channels);
        drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, pcmData.data());
        drwav_uninit(&wav);

        ALenum format = (wav.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
        alBufferData(m_Buffer, format, pcmData.data(), static_cast<ALsizei>(pcmData.size() * sizeof(drwav_int16)), wav.sampleRate);

        alSourcei(m_Source, AL_BUFFER, m_Buffer);
        return (alGetError() == AL_NO_ERROR);
    }

    bool Sound::loadFLAC(const std::string& filepath) {
        drflac* flac = drflac_open_file(filepath.c_str(), NULL);
        if (!flac) {
            return false;
        }

        std::vector<drflac_int16> pcmData(flac->totalPCMFrameCount * flac->channels);
        drflac_read_pcm_frames_s16(flac, flac->totalPCMFrameCount, pcmData.data());
        drflac_close(flac);

        ALenum format = (flac->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
        alBufferData(m_Buffer, format, pcmData.data(), static_cast<ALsizei>(pcmData.size() * sizeof(drflac_int16)), flac->sampleRate);

        alSourcei(m_Source, AL_BUFFER, m_Buffer);
        return (alGetError() == AL_NO_ERROR);
    }

    bool Sound::loadMP3(const std::string& filepath) {
        drmp3 mp3;
        if (!drmp3_init_file(&mp3, filepath.c_str(), NULL)) {
            return false;
        }

        std::vector<drmp3_int16> pcmData(drmp3_get_pcm_frame_count(&mp3) * mp3.channels);
        drmp3_read_pcm_frames_s16(&mp3, drmp3_get_pcm_frame_count(&mp3), pcmData.data());
        drmp3_uninit(&mp3);

        ALenum format = (mp3.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
        alBufferData(m_Buffer, format, pcmData.data(), static_cast<ALsizei>(pcmData.size() * sizeof(drmp3_int16)), mp3.sampleRate);

        alSourcei(m_Source, AL_BUFFER, m_Buffer);
        return (alGetError() == AL_NO_ERROR);
    }

    bool Sound::loadSound(const std::string& filepath) {
        this->m_Filepath = filepath;
        m_IsBufferLoaded = false;

        bool result = false;
        if (filepath.find(".wav") != std::string::npos) {
            result = loadWAV(filepath);
        } else if (filepath.find(".flac") != std::string::npos) {
            result = loadFLAC(filepath);
        } else if (filepath.find(".mp3") != std::string::npos) {
            result = loadMP3(filepath);
        } else {
            std::cerr << "Unsupported audio format: " << filepath << std::endl;
            return false;
        }

        m_IsBufferLoaded = result;
        return result;
    }

    void Sound::play(bool loop) {
        if (!isPlaying()) {
            if (!m_IsBufferLoaded) {
                std::cerr << "No sound loaded. Cannot play." << std::endl;
                return;
            }
            m_IsLooping = loop;
            alSourcei(m_Source, AL_LOOPING, m_IsLooping ? AL_TRUE : AL_FALSE);
            if (m_CurrentPos > 0.0f) {
                alSourcef(m_Source, AL_SEC_OFFSET, m_CurrentPos);
            }
            alSourcePlay(m_Source);
        }
    }

    void Sound::pause() {
        if (isPlaying()) {
            if (!m_IsBufferLoaded) {
                std::cerr << "No sound loaded. Cannot pause." << std::endl;
                return;
            }
            alGetSourcef(m_Source, AL_SEC_OFFSET, &m_CurrentPos);
            alSourcePause(m_Source);
        }
    }

    void Sound::stop() {
        if (!m_IsBufferLoaded) {
            std::cerr << "No sound loaded. Cannot stop." << std::endl;
            return;
        }
        alSourceStop(m_Source);
    }

    bool Sound::isPlaying() const {
        if (!m_IsBufferLoaded) {
            return false;
        }
        ALint state;
        alGetSourcei(m_Source, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }

    void Sound::setVolume(float volume) {
        if (!m_IsBufferLoaded) {
            std::cerr << "No sound loaded. Cannot set volume." << std::endl;
            return;
        }
        alSourcef(m_Source, AL_GAIN, volume);
    }

    void Sound::setFilepath(const std::string& filepath) {

        if (isPlaying()) {
            stop();
        }

        if (loadSound(filepath)) {
            SP_CORE_INFO("Audio loaded successfully from: ", filepath);
        } else {
            std::cerr << "Failed to load sound file: " << filepath << std::endl;
        }
    }

    void Sound::setFilepath(const char* filepath) {
        setFilepath(std::string(filepath));
    }

}