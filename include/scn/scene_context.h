#pragma once

#include "sys/save_data.h"
#include "sys/text_generators.h"

#include "ibn_observer.h"
#include "ibn_transitions.h"

#include "ldtk_gen_enums.h"

namespace bq::scn
{

class scene_stack;

class scene_context final
{
public:
    scene_context(scene_stack& st) : _scene_stack(st)
    {
    }

    scene_context(const scene_context&) = delete;
    scene_context& operator=(const scene_context&) = delete;

private:
    scene_stack& _scene_stack;

    sys::save_data _save_data;
    ibn::transitions _transitions;
    sys::text_generators _text_generators;

    ibn::subject<void(ldtk::gen::lang)> _lang_changed;

public:
    auto stack() -> decltype((_scene_stack))
    {
        return _scene_stack;
    }

    auto stack() const -> decltype((_scene_stack))
    {
        return _scene_stack;
    }

    auto save_data() -> decltype((_save_data))
    {
        return _save_data;
    }

    auto save_data() const -> decltype((_save_data))
    {
        return _save_data;
    }

    auto transitions() -> decltype((_transitions))
    {
        return _transitions;
    }

    auto transitions() const -> decltype((_transitions))
    {
        return _transitions;
    }

    auto text_generators() -> decltype((_text_generators))
    {
        return _text_generators;
    }

    auto text_generators() const -> decltype((_text_generators))
    {
        return _text_generators;
    }

    auto lang_changed() -> decltype((_lang_changed))
    {
        return _lang_changed;
    }
};

} // namespace bq::scn
