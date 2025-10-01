#pragma once

#include "scn/game.h"
#include "scn/lang_select.h"
#include "scn/splash.h"
#include "scn/title.h"
#include "scn/title_license_print.h"
#include "scn/title_licenses_list.h"
#include "scn/title_options.h"

#include <algorithm>

namespace bq::scn
{

inline constexpr int MAX_SCENE_SIZE = std::max({
    sizeof(lang_select),
    sizeof(splash),
    sizeof(title),
    sizeof(title_options),
    sizeof(title_licenses_list),
    sizeof(title_license_print),
    sizeof(game),
});

inline constexpr int MAX_SCENE_ALIGN = std::max({
    alignof(lang_select),
    alignof(splash),
    alignof(title),
    alignof(title_options),
    alignof(title_licenses_list),
    alignof(title_license_print),
    alignof(game),
});

} // namespace bq::scn
