#include <iostream>

#include "include/model.h"

using namespace Exile::Model;
using namespace std;

Map::Map()
{
}

Map::Map(int w, int h)
{
	this->tiles = tiles;

	map = new MapTileArray(boost::extents[w][h]);

	width = w;
	height = h;
}

void Map::generate()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			MapTile *mapTile = new MapTile(i, j, tiles[rand()%tiles.size()], this);
			(*map)[i][j] = mapTile;
		}
	}
}

void Map::setObjectLocation(Object *object, int i, int j)
{
	setObjectLocation(object, (*map)[i][j]);
}

void Map::setObjectLocation(Object *object, MapTile *tile)
{
	MapTile *ol = object->getLocation();
	if (ol == NULL) ol = tile;
	world->handleEvent(new ObjectMoveEvent(object, ol, tile));
}

MapTile *Map::getTile(int i, int j)
{
	if (i < 0 || i >= width || j < 0 || j >= height)
		return NULL;
	else
		return (*map)[i][j];
}
