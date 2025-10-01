#include "scn/title_licenses_list.h"

#include "scn/scene_context.h"
#include "scn/scene_stack.h"
#include "sys/configs.h"

#include <bn_display.h>
#include <bn_keypad.h>

namespace bq::scn
{

namespace
{

constexpr auto FONT = sys::text_generators::font::GALMURI_7;

constexpr int COLUMNS = 2;

constexpr bn::fixed_point TOP_LEFT_NAME_POS(10, 5);
constexpr bn::fixed SPACING_X = bn::display::width() / COLUMNS - TOP_LEFT_NAME_POS.x() / COLUMNS;
constexpr bn::fixed SPACING_Y = 10;

constexpr auto get_pos(int license_idx) -> bn::fixed_point
{
    const int row = license_idx / COLUMNS;
    const int column = license_idx % COLUMNS;

    return bn::fixed_point{
        TOP_LEFT_NAME_POS.x() + column * SPACING_X,
        TOP_LEFT_NAME_POS.y() + row * SPACING_Y,
    };
}

} // namespace

title_licenses_list::title_licenses_list(int license_idx, scene_context& ctx) : scene(ctx), _cursor_idx(license_idx)
{
}

void title_licenses_list::enter()
{
    auto& ctx = context();

    auto& gens = ctx.text_generators();
    auto& gen = gens.get(FONT);

    const auto prev_color = gens.text_color(FONT);
    const auto prev_alignment = gen.alignment();
    gen.set_left_alignment();

    for (int license_idx = 0; license_idx < gen::LICENSE_NAMES.size(); ++license_idx)
    {
        _name_start_idxes[license_idx] = static_cast<std::uint8_t>(_names_sprites.size());

        gens.set_text_color(FONT, license_idx == _cursor_idx ? sys::TEXT_HIGHLIGHT_COLOR : sys::TEXT_NORMAL_COLOR);

        gen.generate_top_left(get_pos(license_idx), gen::LICENSE_NAMES[license_idx], _names_sprites);
    }
    _name_start_idxes.back() = static_cast<std::uint8_t>(_names_sprites.size());

    gen.set_alignment(prev_alignment);
    gens.set_text_color(FONT, prev_color);
}

bool title_licenses_list::update()
{
    if (bn::keypad::up_pressed() || bn::keypad::down_pressed() || bn::keypad::left_pressed() ||
        bn::keypad::right_pressed())
    {
        const int prev_cursor_idx = _cursor_idx;

        if (bn::keypad::up_pressed())
            move_cursor_idx(-COLUMNS);
        if (bn::keypad::down_pressed())
            move_cursor_idx(+COLUMNS);
        if (bn::keypad::left_pressed())
            move_cursor_idx(-1);
        if (bn::keypad::right_pressed())
            move_cursor_idx(+1);

        recolor_license(prev_cursor_idx);
        recolor_license(_cursor_idx);
    }

    if (bn::keypad::a_pressed())
    {
        auto& scene_stack = context().stack();

        scene_stack.reserve_pop();
        scene_stack.reserve_push<title_license_print>(_cursor_idx, context());
    }
    else if (bn::keypad::b_pressed())
    {
        auto& scene_stack = context().stack();

        scene_stack.reserve_pop();
        scene_stack.reserve_push<title_options>(ldtk::gen::title_options_menu::licenses, context());
    }

    return false;
}

void title_licenses_list::move_cursor_idx(int diff)
{
    _cursor_idx = (_cursor_idx + diff + gen::LICENSE_NAMES.size()) % gen::LICENSE_NAMES.size();
}

void title_licenses_list::recolor_license(int license_idx)
{
    bn::array<bn::color, 16> colors{};
    colors[1] = license_idx == _cursor_idx ? sys::TEXT_HIGHLIGHT_COLOR : sys::TEXT_NORMAL_COLOR;

    for (int spr_idx = _name_start_idxes[license_idx]; spr_idx < _name_start_idxes[license_idx + 1]; ++spr_idx)
        _names_sprites[spr_idx].set_palette(bn::sprite_palette_item(colors, bn::bpp_mode::BPP_4));
}

} // namespace bq::scn
