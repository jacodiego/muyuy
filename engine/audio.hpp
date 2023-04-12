#pragma once

#include "utils/singleton.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <bit>
#include <fstream>
#include <AL/alure.h>

namespace muyuy::audio
{
    class AudioEngine;
    extern AudioEngine *audioManager;

    class AudioEngine : public utils::Singleton<AudioEngine>
    {
        friend class utils::Singleton<AudioEngine>;

    public:
        ~AudioEngine();

        bool singletonInitialize() { return true; };

    private:
        AudioEngine();
        bool getAvailableDevices(std::vector<std::string> &, ALCdevice *);

    private:
        // ALCdevice *_device;
        // ALCcontext *_context;
        ALuint _source;
        ALuint _buffer;
        ALCenum _error;
    };
}
