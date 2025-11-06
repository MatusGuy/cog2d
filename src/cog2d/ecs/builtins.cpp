#include "builtins.hpp"

#include "cog2d/ecs/world.hpp"

namespace cog2d::systems {

void velocity_update(EntityBase& ent, CompCollision& col)
{
	if (ent.builtins & COMP_COLLISION) {
		col.mov = ent.vel;
		if (ent.follow_camera) {
			// log::debug(fmt::format("{}", ext::viewport.delta));
			col.mov += ext::viewport.delta;
		}
	} else {
		Vector inc = ent.vel;
		if (ent.follow_camera)
			inc += ext::viewport.delta;

		ent.bbox.pos += inc * velocity_multiplier();
	}
}

}  //namespace cog2d::systems
