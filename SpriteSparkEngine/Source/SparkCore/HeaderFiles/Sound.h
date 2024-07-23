#pragma once

#include "Sparkpch.h"

#include <al.h>
#include <alc.h>

#include "SparkCore/HeaderFiles/GlobalThreadPool.h"

namespace SpriteSpark {

    class Sound {

    public:

        Sound();
        Sound(const std::string& filepath);
        Sound(const char* filepath);
        ~Sound();

        void play(bool loop = false);
        void pause();
        void stop();
        bool isPlaying() const;
        void setVolume(float volume);
        void setFilepath(const std::string& filepath);
        void setFilepath(const char* filepath);
        std::string getFilepath() { return m_Filepath; };

    private:

        void initOpenAL();
        void cleanupOpenAL();

        bool loadWAV(const std::string& filepath);
        bool loadFLAC(const std::string& filepath);
        bool loadMP3(const std::string& filepath);
        bool loadSound(const std::string& filepath);

        ALuint m_Buffer;
        ALuint m_Source;
        ALCdevice* m_Device;
        ALCcontext* m_Context;
        ALfloat m_CurrentPos;
        std::string m_Filepath;
        bool m_IsLooping;
        bool m_IsBufferLoaded;
    };

}