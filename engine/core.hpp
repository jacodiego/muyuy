#include <SDL2/SDL.h>

#include "engine/audio.hpp"
#include "engine/game.hpp"
#include "engine/input.hpp"
#include "engine/screen.hpp"
#include "engine/script.hpp"
#include "engine/system.hpp"
#include "engine/video/video.hpp"

#include "screens/boot/boot.hpp"

namespace muyuy
{
    class Core
    {
    public:
        void run();

    private:
        SDL_Event event;
        static void initializeEngine();
        static bool loadSettings();
    };
}