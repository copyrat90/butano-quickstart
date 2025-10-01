#pragma once

#include "ldtk_gen_enums.h"
#include "ldtk_gen_project.h"

#include <bn_colors.h>

namespace bq::sys
{

constexpr auto get_config_entity(ldtk::gen::entity_ident config_entity_ident) -> const ldtk::entity&
{
    static constexpr auto CONFIG_ENTITIES = ldtk::gen::gen_project.get_level(ldtk::gen::level_ident::configs)
                                                .get_layer(ldtk::gen::layer_ident::entities)
                                                .entity_instances();

    const auto iter = std::ranges::find_if(CONFIG_ENTITIES, [config_entity_ident](const ldtk::entity& entity) {
        return entity.identifier() == config_entity_ident;
    });

    BN_ASSERT(iter != CONFIG_ENTITIES.cend(), "config entity not found");
    return *iter;
}

inline constexpr bn::color TEXT_HIGHLIGHT_COLOR = bn::colors::yellow;
inline constexpr bn::color TEXT_NORMAL_COLOR = bn::colors::white;

} // namespace bq::sys
