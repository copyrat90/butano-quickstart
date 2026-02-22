#include "scn/scene_context.h"
#include "scn/scene_stack.h"

#include "ldtk_core.h"

#include "ibn_stats.h"

#include <bn_common.h>
#include <bn_core.h>

int main()
{
    bn::core::init();
    ldtk::core::init();

    static BN_DATA_EWRAM bq::scn::scene_stack scene_stack;
    static BN_DATA_EWRAM bq::scn::scene_context scene_context(scene_stack);

    if (scene_context.config_save().load())
        scene_stack.reserve_push<bq::scn::splash>(scene_context);
    else
        scene_stack.reserve_push<bq::scn::lang_select>(scene_context);

    while (true)
    {
        scene_stack.update();
        scene_context.transitions().update();

        bn::core::update();
        IBN_STATS_UPDATE;
    }
}
