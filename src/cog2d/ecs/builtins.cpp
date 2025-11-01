#include "builtins.hpp"

namespace cog2d::system {

void velocity(EntityBase& ent, CompCollision& col)
{
	if (ent.builtins & COMP_COLLISION) {
		col.mov = ent.vel;
		//if (ent.follow_camera)
		//	col.mov += viewport.get_camera()->m_delta;
	} else {
		Vector inc = ent.vel;
		//if (follow_camera())
		//	inc += viewport.get_camera()->m_delta;

		ent.bbox.pos += inc * velocity_multiplier();
	}
}

}  //namespace cog2d::system
