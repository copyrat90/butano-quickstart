#pragma once

#include "scn/scene.h"

namespace bq::scn
{

class game final : public scene
{
public:
    game(scene_context&);

public:
    bool update() override;
};

} // namespace bq::scn
