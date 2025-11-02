#include "builtins.hpp"

#include "cog2d/ecs/world.hpp"

namespace cog2d::systems {

void velocity_update(EntityBase& ent, CompCollision& col)
{
	if (ent.builtins & COMP_COLLISION && false) {
		col.mov = ent.vel;
		//if (ent.follow_camera)
		//	col.mov += viewport.get_camera()->m_delta;
	} else {
		Vector inc = ent.vel;
		if (ent.follow_camera)
			inc += ext::viewport.delta;

		ent.bbox.pos += inc * velocity_multiplier();
	}
}

}  //namespace cog2d::systems
