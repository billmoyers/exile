#include <iostream>

#include "include/model.h"

using namespace Exile::Model;
using namespace std;

Map::Map()
{
}

Map::Map(std::vector<Tile *> tiles, int w, int h, World *world)
{
	cout << __FUNCTION__ << ": " << tiles.size() << " tiles, geometry=" << w << "x" << h << endl;

	this->tiles = tiles;

	map = new MapTileArray(boost::extents[w][h]);
	
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			MapTile *mapTile = new MapTile(tiles[rand()%tiles.size()]);
			(*map)[i][j] = mapTile;
		}
	}
	
	this->world = world;
}

void Map::setObjectLocation(Object *object, int i, int j)
{
	(*map)[i][j]->addObject(object);
}
