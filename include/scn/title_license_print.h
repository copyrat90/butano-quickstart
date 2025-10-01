#pragma once

#include "scn/scene.h"

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "ibn_sprite_text_typewriter.h"

namespace bq::scn
{

class title_license_print final : public scene
{
public:
    title_license_print(int license_idx, scene_context&);
    void enter() override;

public:
    bool update() override;

private:
    bn::vector<bn::sprite_ptr, 128> _text_sprites;
    ibn::sprite_text_typewriter _typewriter;

    int _license_idx;
};

} // namespace bq::scn
