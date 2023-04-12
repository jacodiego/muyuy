#pragma once

#include "engine/system.hpp"
#include "engine/video/video.hpp"
#include "utils/position_2d.hpp"

#include <string>
#include <vector>

namespace muyuy::common
{
    enum class DialogueState
    {
        Inactive,
        Writing,
        Option
    };

    class Speaker
    {
    public:
        std::string name;
    };

    class Dialogue
    {
    public:
        explicit Dialogue(const std::string &dialogue_id);

        virtual ~Dialogue();

        static Dialogue *create(DialogueSupervisor *dialogue_supervisor, const std::string &dialogue_id);

        void addLine(const std::string &text);
        void addLine(const std::string &text, int32_t next_line);
        void addLine(const std::string &text, const std::string &speaker_id);
        void addLine(const std::string &text, const std::string &speaker_id, int32_t next_line);
        void addLineTimed(const std::string &text, uint32_t display_time);
        void addLineTimed(const std::string &text, int32_t next_line, uint32_t display_time);
        void addLineTimed(const std::string &text, const std::string &speaker_id, uint32_t display_time);
        void addLineTimed(const std::string &text, const std::string &speaker_id, int32_t next_line, uint32_t display_time);
        void addOption(const std::string &text);
        void addOption(const std::string &text, int32_t next_line);
        bool validate();
        std::string getLineText(uint32_t line) const
        {
            if (line >= _line_count)
                return "";
            return _text[line];
        }

        int32_t getLineNextLine(uint32_t line) const
        {
            if (line >= _line_count)
                return -1;
            return _next_lines[line];
        }

        int32_t getLineDisplayTime(uint32_t line) const
        {
            if (line >= _line_count)
                return -1;
            return _display_times[line];
        }

        DialogueOptions *getLineOptions(uint32_t line) const
        {
            if (line >= _line_count)
                return nullptr;
            return _options[line];
        }
        const std::string &getLineSpeaker(uint32_t line) const
        {
            if (line >= _line_count)
                return "";
            return _speakers[line];
        }

        const std::string &getDialogueID() const
        {
            return _dialogue_id;
        }

        uint32_t getLineCount() const
        {
            return _line_count;
        }

    protected:
        std::string _dialogue_id;
        uint32_t _line_count;
        std::vector<std::string> _text;
        std::vector<int32_t> _next_lines;
        std::vector<int32_t> _display_times;
        std::vector<DialogueOptions *> _options;
        std::vector<std::string> _speakers;
    };

    class DialogueOptions
    {

    public:
        DialogueOptions()
        {
        }

        virtual ~DialogueOptions()
        {
        }

        virtual void addOption(const std::string &text);

        virtual void addOption(const std::string &text, int32_t next_line);

        std::string getOptionText(uint32_t option) const
        {
            if (option >= getNumberOptions())
                return "";
            return _text[option];
        }
        int32_t getOptionNextLine(uint32_t option) const
        {
            if (option >= getNumberOptions())
                return -1;
            return _next_lines[option];
        }
        uint32_t getNumberOptions() const
        {
            return _text.size();
        }

    protected:
        std::vector<std::string> _text;
        std::vector<int32_t> _next_lines;
    };

    class DialogueWindow
    {
    public:
        DialogueWindow();

        ~DialogueWindow()
        {
        }
        void setPosition(float pos_x, float pos_y);

        void clear();

        void draw();

        // vt_gui::TextBox &getDisplayTextBox()
        // {
        //     return _display_textbox;
        // }

        // vt_gui::OptionBox &getDisplayOptionBox()
        // {
        //     return _display_optionbox;
        // }

        // vt_video::TextImage &getNameText()
        // {
        //     return _name_text;
        // }

        bool setIndicator(uint8_t type)
        {
            if (type == _indicator_symbol)
                return false;
            _indicator_symbol = type;
            return true;
        }

    private:
        utils::Position2D _pos;
        video::Texture *_background_box;
        //  vt_video::StillImage _nameplate_image;
        //  vt_video::StillImage _next_line_image;
        //  vt_video::StillImage _last_line_image;
        uint8_t _indicator_symbol;
        uint16_t _blink_time;
        bool _blink_state;
        // vt_gui::TextBox _display_textbox;
        // vt_gui::OptionBox _display_optionbox;
        // vt_video::TextImage _name_text;
    };

    class DialogueSupervisor
    {
    public:
        DialogueSupervisor();
        virtual ~DialogueSupervisor();
        virtual void update();
        virtual void draw();
        virtual void addSpeaker(const std::string &speaker_id, const std::string &name, const std::string &portrait);
        virtual void changeSpeakerName(const std::string &speaker_id, const std::string &name);
        virtual void addDialogue(Dialogue *dialogue);
        virtual void startDialogue(const std::string &dialogue_id);
        virtual void endDialogue();
        virtual void forceNextLine();
        Dialogue *getDialogue(const std::string &dialogue_id);
        Speaker *getSpeaker(const std::string &speaker_id);
        Dialogue *getCurrentDialogue() const
        {
            return _current_dialogue;
        }
        bool isDialogueActive() const
        {
            return (_current_dialogue != nullptr);
        }
        DialogueOptions *getCurrentOptions() const
        {
            return _current_options;
        }
        system::SystemTimer &getLineTimer()
        {
            return _line_timer;
        }
        uint32_t getLineCounter() const
        {
            return _line_counter;
        }
        void setDialoguePosition(float x, float y);

    protected:
        DialogueState _state;
        std::map<std::string, Speaker> _speakers;
        std::map<std::string, Dialogue *> _dialogues;
        Dialogue *_current_dialogue;
        DialogueOptions *_current_options;
        system::SystemTimer _line_timer;
        uint32_t _line_counter;
        DialogueWindow _dialogue_window;
        void _updateLine();
        void _updateOptions();
        void _beginLine();
        void _endLine();
    };

}