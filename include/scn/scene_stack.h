#pragma once

#include "scn/scene_ptr.h"

#include <bn_generic_pool.h>
#include <bn_vector.h>

#include <concepts>
#include <utility>

namespace bq::scn
{

class scene_stack final
{
public:
    scene_stack() = default;

    scene_stack(const scene_stack&) = delete;
    scene_stack& operator=(const scene_stack&) = delete;

public:
    void update();

public:
    template <std::derived_from<scene> Scene, typename... Args>
    void reserve_push(Args&&... args)
    {
        _reserved.emplace_back(&_scene_pool.create<Scene>(std::forward<Args>(args)...), scene_deleter(_scene_pool));
        _reserved_types.push_back(bn::type_id<Scene>());
    }

    void reserve_pop();
    void reserve_clear();

private:
    scene_pool_t _scene_pool;
    bn::vector<scene_ptr, MAX_SCENE_COUNT> _scenes;

    bn::vector<scene_ptr, MAX_SCENE_COUNT> _reserved;
    bn::vector<bn::type_id_t, MAX_SCENE_COUNT> _reserved_types;
};

} // namespace bq::scn
