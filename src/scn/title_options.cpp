#include "scn/title_options.h"

#include "scn/scene_context.h"
#include "scn/scene_stack.h"
#include "sys/configs.h"

#include <bn_colors.h>
#include <bn_keypad.h>

namespace bq::scn
{

namespace
{

constexpr auto HEADING_FONT = sys::text_generators::font::GALMURI_11;
constexpr auto MENU_FONT = sys::text_generators::font::GALMURI_11;

constexpr auto TITLE_MENUS = sys::get_config_entity(ldtk::gen::entity_ident::system_texts)
                                 .get_field(ldtk::gen::entity_field_ident::ENTITY_system_texts_FIELD_title_menus)
                                 .get<bn::span<const bn::string_view>>();

constexpr auto LANG_NAMES = sys::get_config_entity(ldtk::gen::entity_ident::system_texts)
                                .get_field(ldtk::gen::entity_field_ident::ENTITY_system_texts_FIELD_lang_names)
                                .get<bn::span<const bn::string_view>>();

constexpr auto BUTTONS = sys::get_config_entity(ldtk::gen::entity_ident::system_texts)
                             .get_field(ldtk::gen::entity_field_ident::ENTITY_system_texts_FIELD_buttons)
                             .get<bn::span<const bn::string_view>>();

static_assert(BUTTONS.size() % (int)ldtk::gen::lang::max_count == 0,
              "buttons are missing for some lang in system_texts.buttons");
constexpr int BUTTONS_MAX_COUNT = BUTTONS.size() / (int)ldtk::gen::lang::max_count;
constexpr int BACK_BUTTON_IDX = 1;

constexpr auto TITLE_OPTIONS_MENUS =
    sys::get_config_entity(ldtk::gen::entity_ident::system_texts)
        .get_field(ldtk::gen::entity_field_ident::ENTITY_system_texts_FIELD_title_options_menus)
        .get<bn::span<const bn::string_view>>();

// `-2`: Exclude `lang` & `back`
static_assert(TITLE_OPTIONS_MENUS.size() ==
                  ((int)ldtk::gen::title_options_menu::max_count - 2) * (int)ldtk::gen::lang::max_count,
              "title options menu is missing in system_texts.title_options_menus");

constexpr bn::fixed MENUS_X = 40;
constexpr bn::array<bn::fixed, (int)ldtk::gen::title_options_menu::max_count + 1> MENUS_Y{
    60,  // Lang: English
    80,  // Licenses
    100, // Back
};

constexpr bn::fixed_point HEADING_POS(20, 20);

} // namespace

title_options::title_options(ldtk::gen::title_options_menu cursor, scene_context& ctx)
    : scene(ctx), _cursor_idx(static_cast<int>(cursor))
{
}

void title_options::enter()
{
    redraw_all();
}

bool title_options::update()
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

        switch (static_cast<ldtk::gen::title_options_menu>(_cursor_idx))
        {
        case ldtk::gen::title_options_menu::lang: {
            auto& config_save = context().config_save();
            config_save.set_next_language();
            config_save.save();
            redraw_all();
            break;
        }
        case ldtk::gen::title_options_menu::licenses:
            scene_stack.reserve_pop();
            scene_stack.reserve_push<scn::title_licenses_list>(0, context());
            break;
        case ldtk::gen::title_options_menu::back:
            back_to_title();
            break;
        default:
            BN_ERROR("Invalid cursor idx: ", _cursor_idx);
        }
    }
    else if (bn::keypad::b_pressed())
    {
        back_to_title();
    }

    return false;
}

void title_options::back_to_title()
{
    auto& scene_stack = context().stack();

    scene_stack.reserve_pop();
    scene_stack.reserve_push<scn::title>(ldtk::gen::title_menu::options, context());
}

void title_options::move_cursor_idx(int diff)
{
    _cursor_idx = (_cursor_idx + diff + (int)ldtk::gen::title_options_menu::max_count) %
                  (int)ldtk::gen::title_options_menu::max_count;
}

void title_options::recolor_menu(int menu_idx)
{
    bn::array<bn::color, 16> colors{};
    colors[1] = menu_idx == _cursor_idx ? sys::TEXT_HIGHLIGHT_COLOR : sys::TEXT_NORMAL_COLOR;

    for (int spr_idx = _menu_start_idxes[menu_idx]; spr_idx < _menu_start_idxes[menu_idx + 1]; ++spr_idx)
        _menus_sprites[spr_idx].set_palette(bn::sprite_palette_item(colors, bn::bpp_mode::BPP_4));
}

void title_options::redraw_all()
{
    _heading_sprites.clear();
    _menus_sprites.clear();

    auto& ctx = context();
    const auto& config_save = ctx.config_save();

    auto& gens = ctx.text_generators();
    auto& gen = gens.get(MENU_FONT);
    auto& gen_head = gens.get(HEADING_FONT);

    const auto prev_color = gens.text_color(MENU_FONT);
    const auto prev_alignment = gen.alignment();
    const auto prev_color_head = gens.text_color(HEADING_FONT);
    const auto prev_align_head = gen_head.alignment();
    gen.set_left_alignment();
    gen_head.set_left_alignment();

    // Heading
    gen_head.generate_top_left(HEADING_POS,
                               TITLE_MENUS[(int)config_save.language() * (int)ldtk::gen::title_menu::max_count +
                                           (int)ldtk::gen::title_menu::options],
                               _heading_sprites);

    // Menus
    for (int menu_idx = 0; menu_idx < (int)ldtk::gen::title_options_menu::max_count; ++menu_idx)
    {
        _menu_start_idxes[menu_idx] = static_cast<std::uint8_t>(_menus_sprites.size());

        gens.set_text_color(MENU_FONT, menu_idx == _cursor_idx ? sys::TEXT_HIGHLIGHT_COLOR : sys::TEXT_NORMAL_COLOR);

        // Lang: English
        if (menu_idx == 0)
        {
            static constexpr bn::string_view LANG_PREFIX = "Lang: ";
            gen.generate_top_left(MENUS_X, MENUS_Y[menu_idx], LANG_PREFIX, _menus_sprites);
            gen.generate_top_left(MENUS_X + gen.width(LANG_PREFIX), MENUS_Y[menu_idx],
                                  LANG_NAMES[(int)config_save.language()], _menus_sprites);
        }
        // Back
        else if (menu_idx == static_cast<int>(ldtk::gen::title_options_menu::max_count) - 1)
        {
            gen.generate_top_left(MENUS_X, MENUS_Y[menu_idx],
                                  BUTTONS[(int)config_save.language() * BUTTONS_MAX_COUNT + BACK_BUTTON_IDX],
                                  _menus_sprites);
        }
        else
        {
            // `-2`: Exclude `lang` & `back`
            // `-1`: Exclude `lang`
            const auto menu_text =
                TITLE_OPTIONS_MENUS[(int)config_save.language() * ((int)ldtk::gen::title_options_menu::max_count - 2) +
                                    (menu_idx - 1)];
            gen.generate_top_left(MENUS_X, MENUS_Y[menu_idx], menu_text, _menus_sprites);
        }
    }
    _menu_start_idxes.back() = static_cast<std::uint8_t>(_menus_sprites.size());

    gen.set_alignment(prev_alignment);
    gens.set_text_color(MENU_FONT, prev_color);
    gen_head.set_alignment(prev_align_head);
    gens.set_text_color(HEADING_FONT, prev_color_head);
}

} // namespace bq::scn
