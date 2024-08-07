#pragma once

#include "engine/video/video.hpp"
#include "engine/video/types.hpp"
#include "utils/size.hpp"
#include "utils/rect.hpp"
#include <vector>
#include <string>
#include <map>
#include <utility>

namespace muyuy::map
{

    class Tileset;
    struct MapTileset
    {
        int first_id;
        Tileset *tileset;
    };

    class Tileset
    {
    public:
        Tileset(std::string, int, int, int, int, int, std::map<int, Rect *>);
        ~Tileset();
        int getArea() const;
        int getLastId() const;
        Size getSize() const;
        Rect *getCollitionBox(int) const;
        video::RenderTile getRenderTile(int, int, int);
        video::Texture *getTexture() { return texture; };

    private:
        video::Texture *texture;
        Size tileSize;
        int columns;
        int rows;
        int index;
        std::vector<Rect> tileClips;
        std::map<int, Rect *> collitions;
    };

}