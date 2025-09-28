#include "ldtk_core.h"

#include "ibn_stats.h"

#include <bn_core.h>

int main()
{
    bn::core::init();
    ldtk::core::init();

    while (true)
    {
        IBN_STATS_UPDATE;
        bn::core::update();
    }
}
