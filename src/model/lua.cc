#include "include/model.h"

using namespace Exile::Model;

static void World::luaBind(LuaState *lua)
{
	module(lua)
	[
		class_<World>("World")
		.def_readwrite("name", &World::name)
		.def_readwrite("desc", &World::desc)
	];
}