#pragma once

#include "scn/scenes.h"

#include <bn_generic_pool.h>

#include <memory>

namespace bq::scn
{

class scene;

inline constexpr int MAX_SCENE_COUNT = 3;

using scene_pool_t = bn::generic_pool<MAX_SCENE_SIZE, MAX_SCENE_COUNT, MAX_SCENE_ALIGN>;

class scene_deleter final
{
public:
    scene_deleter() = default;
    scene_deleter(scene_pool_t&);

    void operator()(scene*) const;

private:
    scene_pool_t* _pool = nullptr;
};

using scene_ptr = std::unique_ptr<scene, scene_deleter>;

} // namespace bq::scn
