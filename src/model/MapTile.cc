#include "include/model.h"

using namespace Exile::Model;

MapTile::MapTile(int i, int j, Tile *tile, Map *map) : 
	i(i), j(j), tile(tile), map(map)
{
}
