#include "scn/scene_stack.h"

namespace bq::scn
{

void scene_stack::update()
{
    // Update scenes
    for (auto iter = _scenes.rbegin(); iter != _scenes.rend(); ++iter)
    {
        // Break if the upper scene don't want to update the scene below
        if (!(*iter)->update())
            break;
    }

    // Apply reserves
    for (int i = 0; i < _reserved.size(); ++i)
    {
        scene_ptr& reserved = _reserved[i];
        const bn::type_id_t reserved_type = _reserved_types[i];

        // If not `nullptr`, it's a push
        if (reserved)
        {
            // Apply pushes
            if (!_scenes.empty())
                _scenes.back()->cover(reserved_type);

            _scenes.push_back(std::move(reserved));
            _scenes.back()->enter();
        }
        // If `nullptr`, it's a pop
        else
        {
            // Apply pops
            _scenes.pop_back();

            if (!_scenes.empty())
                _scenes.back()->uncover();
        }
    }
    _reserved.clear();
    _reserved_types.clear();
}

void scene_stack::reserve_pop()
{
    // `nullptr` means pop
    _reserved.emplace_back(nullptr);
    _reserved_types.push_back(bn::type_id_t{});
}

void scene_stack::reserve_clear()
{
    for (int i = 0; i < _scenes.size(); ++i)
        reserve_pop();
}

} // namespace bq::scn
