#include "dialogue.hpp"

namespace muyuy::common
{
    Dialogue::Dialogue(const std::string &dialogue_id) : _dialogue_id(dialogue_id),
                                                         _line_count(0)
    {
    }

    Dialogue::~Dialogue()
    {
        for (uint32_t i = 0; i < _options.size(); i++)
        {
            if (_options[i] != nullptr)
            {
                delete _options[i];
                _options[i] = nullptr;
            }
        }
    }

    Dialogue *Dialogue::create(DialogueSupervisor *dialogue_supervisor, const std::string &dialogue_id)
    {
        Dialogue *dialogue = new Dialogue(dialogue_id);
        dialogue_supervisor->addDialogue(dialogue);
        return dialogue;
    }

    void Dialogue::addLine(const std::string &text)
    {
        addLine(text, -1);
    }

    void Dialogue::addLine(const std::string &text, int32_t next_line)
    {
        ++_line_count;
        _text.push_back(text);
        _next_lines.push_back(next_line);
        _display_times.push_back(-1);
        _options.push_back(nullptr);
    }

    void Dialogue::addLine(const std::string &text, const std::string &speaker_id)
    {
        addLineTimed(text, speaker_id, -1, -1);
    }

    void Dialogue::addLine(const std::string &text, const std::string &speaker_id, int32_t next_line)
    {
        addLineTimed(text, speaker_id, next_line, -1);
    }

    void Dialogue::addLineTimed(const std::string &text, uint32_t display_time)
    {
        addLineTimed(text, -1, display_time);
    }

    void Dialogue::addLineTimed(const std::string &text, int32_t next_line, uint32_t display_time)
    {
        ++_line_count;
        _text.push_back(text);
        _next_lines.push_back(next_line);
        _display_times.push_back(display_time);
        _options.push_back(nullptr);
    }

    void Dialogue::addLineTimed(const std::string &text, const std::string &speaker_id, uint32_t display_time)
    {
        addLineTimed(text, speaker_id, -1, display_time);
    }

    void Dialogue::addLineTimed(const std::string &text, const std::string &speaker_id, int32_t next_line, uint32_t display_time)
    {
        addLineTimed(text, next_line, display_time);
        _speakers.push_back(speaker_id);
    }

    void Dialogue::addOption(const std::string &text)
    {
        addOption(text, -1);
    }

    void Dialogue::addOption(const std::string &text, int32_t next_line)
    {
        if (_line_count == 0)
        {
            // IF_PRINT_WARNING(COMMON_DEBUG) << "Attempted to add an option to a dialogue with no lines" << std::endl;
            return;
        }

        uint32_t current_line = _line_count - 1;

        if (_options[current_line] == nullptr)
        {
            _options[current_line] = new DialogueOptions();
        }
        _options[current_line]->addOption(text, next_line);
    }

    bool Dialogue::validate()
    {
        // Valid dialogues need to have at least one line
        if (_line_count == 0)
        {
            // IF_PRINT_WARNING(COMMON_DEBUG) << "Validation failed for dialogue #" << _dialogue_id << ": no lines" << std::endl;
            return false;
        }

        // Check that all next lines with positive values point to valid indeces
        for (uint32_t i = 0; i < _line_count; ++i)
        {
            if ((_next_lines[i] >= 0) && (static_cast<uint32_t>(_next_lines[i]) >= _line_count))
            {
                // IF_PRINT_WARNING(COMMON_DEBUG) << "Validation failed for dialogue #" << _dialogue_id
                //                                << ": next line referred to an invalid line index: " << _next_lines[i] << std::endl;
                return false;
            }

            // If this line has options, we have to examine the next line argument for each option as well
            if (_options[i] != 0)
            {
                if (_options[i]->getNumberOptions() == 0)
                {
                    // PRINT_WARNING << "Validation failed for dialogue #" << _dialogue_id
                    //               << ": line had options declared but no options defined" << std::endl;
                    return false;
                }

                for (uint32_t j = 0; j < _options[i]->getNumberOptions(); ++j)
                {
                    int32_t option_next_line = _options[i]->getOptionNextLine(j);
                    if ((option_next_line >= 0) && (static_cast<uint32_t>(option_next_line) >= _line_count))
                    {
                        // IF_PRINT_WARNING(COMMON_DEBUG) << "Validation failed for dialogue #" << _dialogue_id
                        //                                << ": option's next line referred to an invalid line index: "
                        //                                << option_next_line << std::endl;
                        return false;
                    }
                }
            }
        }

        // Construct containers that hold all unique sprite and event ids for this dialogue
        std::set<std::string> speaker_ids;
        for (uint32_t i = 0; i < _line_count; ++i)
        {
            speaker_ids.insert(_speakers[i]);
        }

        return true;
    }

    void DialogueOptions::addOption(const std::string &text)
    {
        addOption(text, -1);
    }

    void DialogueOptions::addOption(const std::string &text, int32_t next_line)
    {
        _text.push_back(text);
        _next_lines.push_back(next_line);
    }

    DialogueWindow::DialogueWindow() : _pos(512.0f, 512.0f),
                                       _indicator_symbol(0),
                                       _blink_time(0),
                                       _blink_state(true)
    {

        _background_box = video::videoManager->createImage("resources/gui/dialogue_box.png");
        // // TODO: Makes this part of the themes
        // if (_parchment_image.Load("data/gui/black_sleet_parch.png") == false)
        //     PRINT_ERROR << "failed to load dialogue image: " << _parchment_image.GetFilename() << std::endl;

        // if (_nameplate_image.Load("data/gui/dialogue_nameplate.png") == false)
        //     PRINT_ERROR << "failed to load dialogue image: " << _nameplate_image.GetFilename() << std::endl;

        // if (_next_line_image.Load("data/gui/dialogue_cont_arrow.png") == false)
        //     PRINT_ERROR << "failed to load dialogue image: " << _next_line_image.GetFilename() << std::endl;

        // if (_last_line_image.Load("data/gui/dialogue_last_ind.png") == false)
        //     PRINT_ERROR << "failed to load dialogue image: " << _last_line_image.GetFilename() << std::endl;

        // VideoManager->PushState();
        // VideoManager->SetStandardCoordSys();

        // _display_textbox.SetDisplaySpeed(SystemManager->GetMessageSpeed());
        // _display_textbox.SetPosition(260.0f, 596.0f);
        // _display_textbox.SetDimensions(640.0f, 126.0f);
        // _display_textbox.SetTextStyle(TextStyle("text20", Color::black, VIDEO_TEXT_SHADOW_LIGHT));
        // _display_textbox.SetDisplayMode(VIDEO_TEXT_FADECHAR);
        // _display_textbox.SetAlignment(VIDEO_X_CENTER, VIDEO_Y_CENTER);
        // _display_textbox.SetTextAlignment(VIDEO_X_LEFT, VIDEO_Y_TOP);

        // _display_optionbox.SetPosition(300.0f, 630.0f);
        // _display_optionbox.SetDimensions(640.0f, 90.0f, 1, 255, 1, 4);
        // _display_optionbox.SetOptionAlignment(VIDEO_X_LEFT, VIDEO_Y_CENTER);
        // _display_optionbox.SetTextStyle(TextStyle("title20", Color::black, VIDEO_TEXT_SHADOW_LIGHT));
        // _display_optionbox.SetSelectMode(VIDEO_SELECT_SINGLE);
        // _display_optionbox.SetCursorOffset(-55.0f, -25.0f);
        // _display_optionbox.SetVerticalWrapMode(VIDEO_WRAP_MODE_NONE);

        // _name_text.SetStyle(TextStyle("title22", Color::black, VIDEO_TEXT_SHADOW_LIGHT));

        // VideoManager->PopState();
    }

    void DialogueWindow::setPosition(float pos_x, float pos_y)
    {
        _pos.x = pos_x;
        _pos.y = pos_y;

        // _display_textbox.setPosition(_pos.x + 80.0f, _pos.y - 110.0f);
        // _display_optionbox.setPosition(_pos.x - 220.0f, _pos.y - 112.0f);
    }

    void DialogueWindow::clear()
    {
        // _display_textbox.ClearText();
        // _display_optionbox.ClearOptions();
        // _name_text.Clear();
        // _portrait_image = nullptr;
    }

    void DialogueWindow::draw()
    {
        // VideoManager->PushState();
        // VideoManager->SetStandardCoordSys();
        // VideoManager->SetDrawFlags(VIDEO_X_CENTER, VIDEO_Y_BOTTOM, VIDEO_BLEND, 0);

        // VideoManager->Move(_pos.x, _pos.y);
        // _parchment_image.Draw();

        // VideoManager->MoveRelative(-370.0f, -45.0f);
        // if (_portrait_image)
        //     _portrait_image->Draw();

        // if (!_name_text.GetString().empty())
        // {
        //     VideoManager->MoveRelative(0.0f, 30.0f);
        //     _nameplate_image.Draw();

        //     VideoManager->MoveRelative(0.0f, -6.0f);
        //     _name_text.Draw();
        // }
        // else
        // {
        //     VideoManager->MoveRelative(0.0f, 24.0f);
        // }

        // VideoManager->MoveRelative(0.0f, 5.0f);
        // _blink_time += SystemManager->GetUpdateTime();
        // if (_blink_time > 500)
        // {
        //     _blink_time -= 500;
        //     _blink_state = _blink_state ? false : true;
        // }

        // if (_indicator_symbol == DIALOGUE_NEXT_INDICATOR && _blink_state)
        // {
        //     VideoManager->MoveRelative(830.0f, 0.0f);
        //     _next_line_image.Draw();
        // }
        // else if (_indicator_symbol == DIALOGUE_LAST_INDICATOR && _blink_state)
        // {
        //     VideoManager->MoveRelative(830.0f, 0.0f);
        //     _last_line_image.Draw();
        // }

        // _display_textbox.Draw();
        // _display_optionbox.Draw();

        // VideoManager->PopState();
    }

    DialogueSupervisor::DialogueSupervisor() : _state(DialogueState::Inactive),
                                               _current_dialogue(nullptr),
                                               _current_options(nullptr),
                                               _line_timer(),
                                               _line_counter(0),
                                               _dialogue_window()
    {
        _dialogue_window.setPosition(512.0f, 170.0f);
    }

    DialogueSupervisor::~DialogueSupervisor()
    {
        // Delete all dialogues
        for (std::map<std::string, Dialogue *>::iterator it = _dialogues.begin(); it != _dialogues.end(); ++it)
        {
            delete it->second;
        }
        _dialogues.clear();
        _speakers.clear();
    }

    void DialogueSupervisor::setDialoguePosition(float x, float y)
    {
        _dialogue_window.setPosition(x, y);
    }

    void DialogueSupervisor::update()
    {
        if (_current_dialogue == nullptr)
            return;

        _line_timer.update();

        switch (_state)
        {
        case DialogueState::Writing:
            _updateLine();
            break;
        case DialogueState::Option:
            _updateOptions();
            break;
        default:
            // PRINT_WARNING << "Dialogue supervisor was in an unknown state: " << _state << std::endl;
            _state = DialogueState::Writing;
            break;
        }
    }

    void DialogueSupervisor::draw()
    {
        _dialogue_window.draw();
    }

    void DialogueSupervisor::addDialogue(Dialogue *dialogue)
    {
        if (dialogue == nullptr)
        {
            // IF_PRINT_WARNING(COMMON_DEBUG) << "function received nullptr argument" << std::endl;
            return;
        }

        if (getDialogue(dialogue->getDialogueID()) != nullptr)
        {
            // IF_PRINT_WARNING(COMMON_DEBUG) << "a dialogue was already registered with this ID: " << dialogue->GetDialogueID() << std::endl;
            delete dialogue;
            return;
        }
        _dialogues.insert(std::make_pair(dialogue->getDialogueID(), dialogue));
    }

    void DialogueSupervisor::addSpeaker(const std::string &speaker_id, const std::string &name, const std::string &portrait)
    {
        if (_speakers.find(speaker_id) != _speakers.end())
        {
            // PRINT_WARNING << "Speaker already existed with requested id: " << speaker_id << std::endl;
            return;
        }

        Speaker new_speaker;
        new_speaker.name = name;
        // if (!portrait.empty())
        // {
        //     if (!new_speaker.portrait.Load(portrait))
        //     {
        //         IF_PRINT_WARNING(COMMON_DEBUG) << "invalid image filename for new portrait: " << portrait << std::endl;
        //     }
        //     // Make sure the portrait doesn't go over the screen edge.
        //     if (new_speaker.portrait.GetHeight() > 130.0f)
        //         new_speaker.portrait.SetHeightKeepRatio(130.0f);
        // }

        _speakers[speaker_id] = new_speaker;
    }

    void DialogueSupervisor::changeSpeakerName(const std::string &speaker_id, const std::string &name)
    {
        std::map<std::string, Speaker>::iterator it = _speakers.find(speaker_id);
        if (it == _speakers.end())
        {
            // PRINT_WARNING << "No speaker found with requested id: " << speaker_id << std::endl;
            return;
        }

        it->second.name = name;

        if (_current_dialogue != nullptr)
        {
            if (_current_dialogue->getLineSpeaker(_line_counter) == speaker_id)
            {
                //_dialogue_window.getNameText().setText(it->second.name);
            }
        }
    }

    // void DialogueSupervisor::ChangeSpeakerPortrait(const std::string &speaker_id, const std::string &portrait)
    // {
    //     if (portrait.empty())
    //         return;

    //     std::map<std::string, Speaker>::iterator it = _speakers.find(speaker_id);
    //     if (it == _speakers.end())
    //     {
    //         PRINT_WARNING << "No speaker found with requested id: " << speaker_id << std::endl;
    //         return;
    //     }

    //     // Note: we don't have to also check whether or not the active portrait on the dialogue window needs to be
    //     // updated since the dialogue window simply retains a pointer to the image object. We only update the StillImage
    //     // class object contents in this function, not its address.
    //     if (!it->second.portrait.Load(portrait))
    //     {
    //         PRINT_WARNING << "Invalid image filename for new portrait: " << portrait << std::endl;
    //         return;
    //     }
    // }

    void DialogueSupervisor::startDialogue(const std::string &dialogue_id)
    {
        Dialogue *dialogue = getDialogue(dialogue_id);

        if (dialogue == nullptr)
        {
            // PRINT_WARNING << "Could not begin dialogue because none existed for id: " << dialogue_id << std::endl;
            return;
        }

        if (_current_dialogue != nullptr)
        {
            // PRINT_WARNING << "beginning a new dialogue while another dialogue is still active" << std::endl;
        }

        _line_counter = 0;
        _current_dialogue = dialogue;
        _current_options = _current_dialogue->getLineOptions(_line_counter);

        _beginLine();
    }

    void DialogueSupervisor::endDialogue()
    {
        if (_current_dialogue == nullptr)
        {
            // IF_PRINT_WARNING(COMMON_DEBUG) << "Tried to end a dialogue when none was active" << std::endl;
            return;
        }

        _current_dialogue = nullptr;
        _current_options = nullptr;
        _line_timer.finish();
    }

    void DialogueSupervisor::forceNextLine()
    {
        if (_current_dialogue == nullptr)
        {
            // IF_PRINT_WARNING(COMMON_DEBUG) << "function called when no dialogue was active" << std::endl;
            return;
        }

        _endLine();
    }

    Dialogue *DialogueSupervisor::getDialogue(const std::string &dialogue_id)
    {
        std::map<std::string, Dialogue *>::iterator it = _dialogues.find(dialogue_id);
        if (it == _dialogues.end())
            return nullptr;

        return it->second;
    }

    Speaker *DialogueSupervisor::getSpeaker(const std::string &speaker_id)
    {
        std::map<std::string, Speaker>::iterator it = _speakers.find(speaker_id);
        if (it != _speakers.end())
            return &(it->second);

        return nullptr;
    }

    void DialogueSupervisor::_updateLine()
    {
        // _dialogue_window.getDisplayTextBox().Update();

        // if (_current_options != nullptr)
        // {
        //     if (_dialogue_window.getDisplayTextBox().IsFinished())
        //     {
        //         _state = DIALOGUE_STATE_OPTION;
        //         return;
        //     }
        // }

        // // If the line has a valid display time and the timer is finished, move on to the next line
        // if ((_line_timer.getDuration() > 0) && (_line_timer.isFinished()))
        // {
        //     _endLine();
        //     return;
        // }

        // // Set the correct indicator
        // if (_current_options || !_dialogue_window.getDisplayTextBox().isFinished())
        // {
        //     _dialogue_window.setIndicator(0);
        // }
        // else if (_line_counter == _current_dialogue->getLineCount() - 1)
        // {
        //     if (_dialogue_window.setIndicator(2))
        //         vt_global::GlobalManager->media().playSound("line_complete");
        // }
        // else
        // {
        //     if (_dialogue_window.setIndicator(1))
        //         vt_global::GlobalManager->media().playSound("line_complete");
        // }

        // // If the current mode is accepting input, we can handle it.
        // if (!vt_mode_manager::ModeManager->getTop()->acceptUserInputInDialogues())
        //     return;

        // if (input::inputManager->confirmPress())
        // {
        //     // If the line is not yet finished displaying, display the rest of the text
        //     if (_dialogue_window.getDisplayTextBox().isFinished() == false)
        //     {
        //         _dialogue_window.getDisplayTextBox().forceFinish();
        //     }
        //     // Proceed to option selection if the line has options
        //     else if (_current_options != nullptr)
        //     {
        //         _state = DialogueState::Option;
        //     }
        //     else
        //     {
        //         _endLine();
        //     }
        // }
    }

    void DialogueSupervisor::_updateOptions()
    {
        // _dialogue_window.getDisplayOptionBox().update();
        // vt_global::GlobalMedia &media = vt_global::GlobalManager->media();

        // if (vt_input::InputManager->ConfirmPress())
        // {
        //     _dialogue_window.GetDisplayOptionBox().InputConfirm();
        //     media.PlaySound("confirm");
        //     _EndLine();
        // }

        // else if (vt_input::InputManager->UpPress())
        // {
        //     _dialogue_window.GetDisplayOptionBox().InputUp();
        //     media.PlaySound("bump");
        // }

        // else if (vt_input::InputManager->DownPress())
        // {
        //     _dialogue_window.GetDisplayOptionBox().InputDown();
        //     media.PlaySound("bump");
        // }
    }

    void DialogueSupervisor::_beginLine()
    {
        // _state = DIALOGUE_STATE_LINE;
        // _current_options = _current_dialogue->GetLineOptions(_line_counter);

        // // Initialize the line timer
        // if (_current_dialogue->GetLineDisplayTime(_line_counter) >= 0)
        // {
        //     _line_timer.Initialize(_current_dialogue->GetLineDisplayTime(_line_counter));
        //     _line_timer.Run();
        // }
        // // If the line has no timer specified, set the line time to zero and put the timer in the finished state
        // else
        // {
        //     _line_timer.Initialize(0);
        //     _line_timer.Finish();
        // }

        // // Setup the text and graphics for the dialogue window
        // _dialogue_window.Clear();
        // _dialogue_window.GetDisplayTextBox().SetDisplayText(_current_dialogue->GetLineText(_line_counter));
        // if (_current_options != nullptr)
        // {
        //     for (uint32_t i = 0; i < _current_options->GetNumberOptions(); ++i)
        //     {
        //         _dialogue_window.GetDisplayOptionBox().AddOption(_current_options->GetOptionText(i));
        //     }

        //     _dialogue_window.GetDisplayOptionBox().SetSelection(0);
        // }

        // Speaker *line_speaker = GetSpeaker(_current_dialogue->GetLineSpeaker(_line_counter));
        // if (line_speaker == nullptr)
        // {
        //     IF_PRINT_WARNING(COMMON_DEBUG) << "dialogue #" << _current_dialogue->GetDialogueID()
        //                                    << " referenced a speaker that did not exist with id: " << _current_dialogue->GetLineSpeaker(_line_counter) << std::endl;
        //     _dialogue_window.GetNameText().SetText("");
        //     _dialogue_window.SetPortraitImage(nullptr);
        // }
        // else
        // {
        //     _dialogue_window.GetNameText().SetText(line_speaker->name);
        //     _dialogue_window.SetPortraitImage(&(line_speaker->portrait));
        // }
    }

    void DialogueSupervisor::_endLine()
    {
        // // Determine the next line to read
        // int32_t next_line = _current_dialogue->getLineNextLine(_line_counter);
        // // If this line had options, the selected option next line overrides the line's next line that we set above
        // if (_current_options != nullptr)
        // {
        //     uint32_t selected_option = _dialogue_window.getDisplayOptionBox().getSelection();
        //     next_line = _current_options->getOptionNextLine(selected_option);
        // }

        // // --- Case 1: Explicitly setting the next line. Warn and end the dialogue if the line to move to is invalid
        // if (next_line >= 0)
        // {
        //     if (static_cast<uint32_t>(next_line) >= _current_dialogue->GetLineCount())
        //     {
        //         IF_PRINT_WARNING(COMMON_DEBUG) << "dialogue #" << _current_dialogue->GetDialogueID()
        //                                        << " tried to set dialogue to invalid line. Current/next line values: {" << _line_counter
        //                                        << ", " << next_line << "}" << std::endl;
        //         next_line = DIALOGUE_END;
        //     }
        // }
        // // --- Case 2: Request to incrementing the current line. If we're incrementing past the last line, end the dialogue
        // else if (next_line == DIALOGUE_NEXT_LINE)
        // {
        //     next_line = _line_counter + 1;
        //     if (static_cast<uint32_t>(next_line) >= _current_dialogue->GetLineCount())
        //         next_line = DIALOGUE_END;
        // }
        // // --- Case 3: Request to end the current dialogue
        // else if (next_line == DIALOGUE_END)
        // {
        //     // Do nothing
        // }
        // // --- Case 4: Unknown negative value. Warn and end dialogue
        // else
        // {
        //     // IF_PRINT_WARNING(COMMON_DEBUG) << "dialogue #" << _current_dialogue->GetDialogueID()
        //     //                                << " unknown next line control value: " << next_line << std::endl;
        //     next_line = DIALOGUE_END;
        // }

        // // Now either end the dialogue or move on to the next line
        // if (next_line == DIALOGUE_END)
        // {
        //     endDialogue();
        // }
        // else
        // {
        //     _line_counter = next_line;
        //     _beginLine();
        // }
    }

}