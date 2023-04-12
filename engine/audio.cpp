#include "audio.hpp"

namespace muyuy::audio
{

    AudioEngine *audioManager = nullptr;

    AudioEngine::AudioEngine()
    {

        ALuint src, buf;

        alureInitDevice(nullptr, nullptr);
        alListener3f(AL_POSITION, 0.f, 0.f, 0.f);
        alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f);
        float ori[6] = {
            0, 0, -1,
            0, 1, 0};

        alListenerfv(AL_ORIENTATION, ori);

        buf = alureCreateBufferFromFile("resources/audio/intro.wav");

        alGenSources(1, &src);
        alSourcei(src, AL_BUFFER, buf);
        alSourcef(src, AL_GAIN, 0.05f);

        //alurePlaySource(src, nullptr, this);
    }

    AudioEngine::~AudioEngine()
    {
        alureShutdownDevice();
    }
}