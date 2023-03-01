#include "animation.hpp"

namespace muyuy::video
{

    Animation::Animation(const std::string &script_file)
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base);

        lua.script_file(script_file);

        sol::table animation = lua["animation"];
        _texture = videoManager->createImage(animation.get<std::string>("image_filename").c_str());
        _cols = animation.get<uint8_t>("columns");
        _rows = animation.get<uint8_t>("rows");
        _width = _texture->getWidth() / _cols;
        _height = _texture->getHeight() / _rows;

        for (const auto &frames : animation.get<sol::table>("frames"))
        {
            sol::object frame_key = frames.first;
            sol::table frame_value = frames.second.as<sol::table>();
            std::vector<AnimationFrame> vec;

            for (int f = 0; f <= frame_value.size(); f++)
            {
                vec.push_back(AnimationFrame{.id = frame_value[f].get<sol::table>().get<uint8_t>("id"),
                                             .duration = frame_value[f].get<sol::table>().get<uint16_t>("duration")});
            }

            _frames.insert(std::make_pair(frame_key.as<std::string>(), vec));
            _last_update = 0;
            _current_index = 0;
            _current_frame = nullptr;
        }
    }

    void Animation::draw(const std::string &frame_key, int x, int y)
    {
        uint32_t frame_time = system::systemManager->getUpdateTime();
        if (_current_frame == nullptr)
            _current_frame = &_frames.at(frame_key)[_current_index];

        if (_last_update > _current_frame->duration)
        {
            _last_update = 0;
            if (_current_index == _frames.at(frame_key).size() - 1)
                _current_index = 0;
            else
                _current_index++;
        }
        _current_frame = &_frames.at(frame_key)[_current_index];

        int id = _current_frame->id - 1;
        _texture->draw(x, y, ((id - _cols) % _cols) * _width, (id / _cols) * _height, _width, _height, 1.0f, 1.0f, 1.0f);
        _last_update += frame_time;
    }
}