#include "scn/scene_ptr.h"

namespace bq::scn
{

#if BQ_CFG_POOL_SCENES

scene_deleter::scene_deleter(scene_pool_t& pool) : _pool(&pool)
{
}

void scene_deleter::operator()(scene* scene_) const
{
    _pool->destroy(*scene_);
}

#endif

} // namespace bq::scn
