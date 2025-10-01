#pragma once

#include "scn/scene.h"

#include <bn_regular_bg_animate_actions.h>

#include <cstdint>

#include "ldtk_gen_enums.h"

namespace bq::scn
{

class splash final : public scene
{
public:
    splash(scene_context&);
    void enter() override;

public:
    bool update() override;

private:
    void update_fade_in();
    void update_waiting();
    void update_fade_out();

    void transit_to_waiting();
    void transit_to_fade_out();

private:
    enum class state : std::uint8_t
    {
        FADE_IN,
        WAITING,
        FADE_OUT,
    };

private:
    bn::regular_bg_cached_animate_action<2> _bg_anim;

    state _state = state::FADE_IN;
    bool _key_ever_pressed = false;

    std::uint16_t _waiting_current_update = 0;
};

} // namespace bq::scn
