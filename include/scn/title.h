#pragma once

#include "scn/scene.h"

#include <bn_array.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "ldtk_gen_enums.h"

#include <cstdint>

namespace bq::scn
{

class title final : public scene
{
public:
    title(ldtk::gen::title_menu cursor, scene_context&);
    void enter() override;

public:
    bool update() override;

private:
    void move_cursor_idx(int diff);

    void recolor_menu(int menu_idx);

private:
    bn::vector<bn::sprite_ptr, 128> _menus_sprites;
    bn::array<std::uint8_t, (int)ldtk::gen::title_menu::max_count + 1> _menu_start_idxes;

    int _cursor_idx;
};

} // namespace bq::scn
