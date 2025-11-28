// HACK: yeah... this is a real issue... I don't know how to fix this effectively without code
// generation

#include <cog2d/program.hpp>
#include <cog2d/ecs/world.hpp>
#include <cog2d/ecs/builtins/collision/collisionsystem.hpp>

static cog2d::Viewport viewport;

namespace cog2d {

namespace program::ext {
void program_settings(ProgramSettings& settings)
{
}
void init()
{
}
bool event(SDL_Event* ev)
{
	return false;
}
void update()
{
}
void draw()
{
}
void quit()
{
}

void load_config(const TomlTable& table)
{
}
void save_config(TomlTable& table)
{
}

void register_actions()
{
}

void register_config()
{
}
}  //namespace program::ext

namespace ext {
Viewport& viewport = ::viewport;

void entity_get_collision(EntityId id, EntityBase** ent, CompCollision** col)
{
}
CollisionResponse entity_collision(EntityBase& a, EntityBase& b)
{
	return COLRESP_REJECT;
}
CollisionResponse entity_collision_tile(EntityBase& base, std::size_t tileidx)
{
	return COLRESP_REJECT;
}
}  //namespace ext

}  //namespace cog2d
