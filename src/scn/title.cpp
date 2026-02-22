#include "scn/title.h"

#include "scn/scene_context.h"
#include "scn/scene_stack.h"
#include "sys/configs.h"

#include <bn_colors.h>
#include <bn_keypad.h>
#include <bn_string_view.h>

namespace bq::scn
{

namespace
{

constexpr auto MENU_FONT = sys::text_generators::font::GALMURI_11;

constexpr auto TITLE_MENUS = sys::get_config_entity(ldtk::gen::entity_ident::system_texts)
                                 .get_field(ldtk::gen::entity_field_ident::ENTITY_system_texts_FIELD_title_menus)
                                 .get<bn::span<const bn::string_view>>();

static_assert(TITLE_MENUS.size() == (int)ldtk::gen::title_menu::max_count * (int)ldtk::gen::lang::max_count,
              "title menu is missing in system_texts.title_menus");

constexpr bn::fixed MENUS_X = 40;
constexpr bn::array<bn::fixed, (int)ldtk::gen::title_menu::max_count> MENUS_Y{
    80,  // Start
    100, // Options
};

} // namespace

title::title(ldtk::gen::title_menu cursor, scene_context& ctx) : scene(ctx), _cursor_idx(static_cast<int>(cursor))
{
    const auto& config_save = ctx.config_save();

    auto& gens = ctx.text_generators();
    auto& gen = gens.get(MENU_FONT);

    const auto prev_color = gens.text_color(MENU_FONT);
    const auto prev_alignment = gen.alignment();
    gen.set_left_alignment();

    for (int menu_idx = 0; menu_idx < (int)ldtk::gen::title_menu::max_count; ++menu_idx)
    {
        _menu_start_idxes[menu_idx] = static_cast<std::uint8_t>(_menus_sprites.size());

        gens.set_text_color(MENU_FONT, menu_idx == _cursor_idx ? sys::TEXT_HIGHLIGHT_COLOR : sys::TEXT_NORMAL_COLOR);

        const auto menu_text =
            TITLE_MENUS[(int)config_save.language() * (int)ldtk::gen::title_menu::max_count + menu_idx];
        gen.generate_top_left(MENUS_X, MENUS_Y[menu_idx], menu_text, _menus_sprites);
    }
    _menu_start_idxes.back() = static_cast<std::uint8_t>(_menus_sprites.size());

    gen.set_alignment(prev_alignment);
    gens.set_text_color(MENU_FONT, prev_color);
}

bool title::update()
{
    if (bn::keypad::up_pressed() || bn::keypad::down_pressed())
    {
        const int prev_cursor_idx = _cursor_idx;

        if (bn::keypad::up_pressed())
            move_cursor_idx(-1);
        if (bn::keypad::down_pressed())
            move_cursor_idx(+1);

        recolor_menu(prev_cursor_idx);
        recolor_menu(_cursor_idx);
    }

    if (bn::keypad::a_pressed())
    {
        auto& scene_stack = context().stack();

        switch (static_cast<ldtk::gen::title_menu>(_cursor_idx))
        {
        case ldtk::gen::title_menu::start:
            scene_stack.reserve_replace_top_with_delay<scn::game>(context());
            break;
        case ldtk::gen::title_menu::options:
            scene_stack.reserve_replace_top_with_delay<scn::title_options>(ldtk::gen::title_options_menu::lang,
                                                                           context());
            break;
        default:
            BN_ERROR("Invalid cursor idx: ", _cursor_idx);
        }
    }

    return false;
}

void title::move_cursor_idx(int diff)
{
    _cursor_idx = (_cursor_idx + diff + (int)ldtk::gen::title_menu::max_count) % (int)ldtk::gen::title_menu::max_count;
}

void title::recolor_menu(int menu_idx)
{
    bn::array<bn::color, 16> colors{};
    colors[1] = menu_idx == _cursor_idx ? sys::TEXT_HIGHLIGHT_COLOR : sys::TEXT_NORMAL_COLOR;

    for (int spr_idx = _menu_start_idxes[menu_idx]; spr_idx < _menu_start_idxes[menu_idx + 1]; ++spr_idx)
        _menus_sprites[spr_idx].set_palette(bn::sprite_palette_item(colors, bn::bpp_mode::BPP_4));
}

} // namespace bq::scn
