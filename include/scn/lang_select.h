#pragma once

#include "scn/scene.h"

#include <bn_array.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "ldtk_gen_enums.h"

#include <cstdint>

namespace bq::scn
{

class lang_select final : public scene
{
public:
    lang_select(scene_context&);
    void enter() override;

public:
    bool update() override;

private:
    void move_cursor_idx(int diff);

    void recolor_lang(ldtk::gen::lang lang);

private:
    bn::vector<bn::sprite_ptr, 128> _texts_sprites;
    bn::array<std::uint8_t, (int)ldtk::gen::lang::max_count + 1> _lang_start_idxes;

    int _cursor_idx = 0;
};

} // namespace bq::scn
