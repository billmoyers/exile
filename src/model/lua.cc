/**
 * @file model/lua.cc
 * Defines functions for the various model classes to interact with Lua.
 **/

#include <iostream>

#include "include/model.h"

#define LuaClassNonDeletable(in_class) \
namespace luabind{namespace detail{ \
template<> struct delete_s<in_class> {static void apply(void* ptr) {}}; \
template<> struct destruct_only_s<in_class> {static void apply(void* ptr){}}; \
}}

using namespace Exile::Model;
using namespace luabind;
using namespace std;

LuaClassNonDeletable(Object)
LuaClassNonDeletable(Entity)
LuaClassNonDeletable(Map)
LuaClassNonDeletable(World)

void Object::luaBind(LuaState *lua)
{
	module(lua, "Exile")
	[
		namespace_("Model")
		[
			class_<Object>("Object")
		]
	];
}

void Entity::luaBind(LuaState *lua)
{
	module(lua, "Exile")
	[
		namespace_("Model")
		[
			class_<Entity, Object>("Entity")
			.def(constructor<>())
		]
	];
}

void Tile::luaBind(LuaState *lua)
{
	module(lua, "Exile")
	[
		namespace_("Model")
		[
			class_<Tile>("Tile")
			.def(constructor<string, Terrain, int>())
			.enum_("constants")
			[
				value("GRASS", GRASS),
				value("SNOW", SNOW),
				value("MUD", MUD),
				value("PAVED", PAVED),
				value("WALL", WALL)
			]
		]
	];	
}

void Map::luaBind(LuaState *lua)
{
	module(lua, "Exile")
	[
		namespace_("Model")
		[
			class_<Map>("Map")
			.def(constructor<int, int>())
			.def("addTile", &Map::addTile)
			.def("generate", &Map::generate)
			.def("setObjectLocation", (void (Map::*)(Object *, int, int)) &Map::setObjectLocation)
		]
	];	
}

void World::luaBind(LuaState *lua)
{
	open(lua);
	
	module(lua, "Exile")
	[
		namespace_("Model")
		[
			class_<World>("World")
			.def_readwrite("name", &World::name)
			.def_readwrite("desc", &World::desc)
			.def("addMap", &World::addMap)
		]
	];
	
	Object::luaBind(lua);
	Entity::luaBind(lua);
	Tile::luaBind(lua);
	Map::luaBind(lua);
}
