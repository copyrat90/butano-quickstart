#include "scn/lang_select.h"

#include "scn/scene_context.h"
#include "scn/scene_stack.h"
#include "sys/configs.h"

#include <bn_colors.h>
#include <bn_fixed.h>
#include <bn_keypad.h>
#include <bn_span.h>
#include <bn_string_view.h>

namespace bq::scn
{

namespace
{

constexpr auto FONT = sys::text_generators::font::GALMURI_11;
constexpr bn::fixed LINE_SPACING = 18;

constexpr auto LANG_NAMES = sys::get_config_entity(ldtk::gen::entity_ident::system_texts)
                                .get_field(ldtk::gen::entity_field_ident::ENTITY_system_texts_FIELD_lang_names)
                                .get<bn::span<const bn::string_view>>();

static_assert(LANG_NAMES.size() == (int)ldtk::gen::lang::max_count,
              "language name is missing in system_texts.lang_names");

} // namespace

lang_select::lang_select(scene_context& ctx) : scene(ctx)
{
    auto& gens = ctx.text_generators();
    auto& gen = gens.get(FONT);

    const auto prev_color = gens.text_color(FONT);
    const auto prev_alignment = gen.alignment();
    gen.set_center_alignment();

    for (int lang_idx = 0; lang_idx < LANG_NAMES.size(); ++lang_idx)
    {
        _lang_start_idxes[lang_idx] = static_cast<std::uint8_t>(_texts_sprites.size());

        gens.set_text_color(FONT, lang_idx == _cursor_idx ? sys::TEXT_HIGHLIGHT_COLOR : sys::TEXT_NORMAL_COLOR);

        const bn::fixed y_idx = lang_idx - bn::fixed(LANG_NAMES.size() - 1) / 2;
        const bn::fixed y = LINE_SPACING * y_idx;
        gen.generate(0, y, LANG_NAMES[lang_idx], _texts_sprites);
    }
    _lang_start_idxes.back() = static_cast<std::uint8_t>(_texts_sprites.size());

    gen.set_alignment(prev_alignment);
    gens.set_text_color(FONT, prev_color);
}

bool lang_select::update()
{
    if (bn::keypad::up_pressed() || bn::keypad::down_pressed())
    {
        const int prev_cursor_idx = _cursor_idx;

        if (bn::keypad::up_pressed())
            move_cursor_idx(-1);
        if (bn::keypad::down_pressed())
            move_cursor_idx(+1);

        recolor_lang(static_cast<ldtk::gen::lang>(prev_cursor_idx));
        recolor_lang(static_cast<ldtk::gen::lang>(_cursor_idx));
    }

    if (bn::keypad::a_pressed())
    {
        auto& ctx = context();
        auto& config_save = ctx.config_save();
        auto& scene_stack = ctx.stack();

        const auto lang = static_cast<ldtk::gen::lang>(_cursor_idx);

        config_save.set_language(lang);
        config_save.save();
        ctx.lang_changed().notify(lang);

        scene_stack.reserve_replace_top_with_delay<scn::splash>(context());
    }

    return false;
}

void lang_select::move_cursor_idx(int diff)
{
    _cursor_idx = (_cursor_idx + diff + LANG_NAMES.size()) % LANG_NAMES.size();
}

void lang_select::recolor_lang(ldtk::gen::lang lang)
{
    const int lang_idx = static_cast<int>(lang);

    bn::array<bn::color, 16> colors{};
    colors[1] = lang_idx == _cursor_idx ? sys::TEXT_HIGHLIGHT_COLOR : sys::TEXT_NORMAL_COLOR;

    for (int spr_idx = _lang_start_idxes[lang_idx]; spr_idx < _lang_start_idxes[lang_idx + 1]; ++spr_idx)
        _texts_sprites[spr_idx].set_palette(bn::sprite_palette_item(colors, bn::bpp_mode::BPP_4));
}

} // namespace bq::scn
