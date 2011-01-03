#include <iostream>
#include "include/model.h"
#include <Ogre.h>

using namespace Exile::Model;
using namespace std;

int Object::nextID = 0;

Object::Object() 
{
	id = Object::getNewID();
}

Object::Object(const std::string &id)
{
	this->id = id;
}

const std::string Object::getNewID()
{
	std::stringstream ss;
	ss << "_object" << Object::nextID++ << endl;
	return ss.str();
}

void Object::handleEvent(Event *event)
{
	AS_BLOCK (ObjectMoveEvent *, ome, event)
	{		
		MapTile *tile = ome->getDest();
		tile->addObject(this);
		location = tile;
	}
}
