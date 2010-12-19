#include "include/model.h"

using namespace Exile::Model;

World::World()
{
	Tile *tile1 = new Tile("grass", GRASS, 0);
	tiles.push_back(tile1);

	Tile *tile2 = new Tile("mud", MUD, 0);
	tiles.push_back(tile2);

	Tile *tile3 = new Tile("snow-heavy", SNOW, 0);
	tiles.push_back(tile3);

	Tile *tile4 = new Tile("snow-light", SNOW, 1);
	tiles.push_back(tile4);

	Tile *tile5 = new Tile("stone-floor", PAVED, 0);
	tiles.push_back(tile5);

	Tile *tile6 = new Tile("stone-wall", WALL, 0);
	tiles.push_back(tile6);
	
	Map *map = new Map(tiles, 10, 10, this);
	maps.push_back(map);
	
	Race *race = new Race("human");
	races.push_back(race);

	Class *cls = new Class("knight");
	classes.push_back(cls);

	for (int i = 0; i < 10; i++)
	{
		Object *entity1 = (Object *) new Entity(race, cls);
		map->setObjectLocation(entity1, i, i);
	}
}

void World::removeObjectListener(ObjectListener *listener)
{
}
