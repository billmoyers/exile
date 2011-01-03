#include <iostream>
#include <boost/foreach.hpp>

#include "include/model.h"

using namespace Exile::Model;
using namespace std;

World::World()
{
	preEventListeners.clear();
	postEventListeners.clear();
}

/**
 * Dispatch an event to its target informing pre- and post-listeners.
 * This is made more complicated because it has to respect the possibility
 * of the listener or dispatch pausing/resuming the event.
 * @param event Event to dispatch.
 **/
void World::handleEvent(Event *event)
{ 
	if (event->getStage() == 0 && event->isActive())
	{
		BOOST_FOREACH(EventListener *listener, preEventListeners)
		{
			if (event->getLastListener() != NULL)
			{
				if (event->getLastListener() != listener)
					continue;
				else
				{
					event->setLastListener(NULL);
					continue;
				}
			}
		
			listener->onEvent(event);
			if (!event->isActive()) 
			{
				event->setLastListener(listener);
				break;
			}
		}
		
		if (event->isActive()) event->nextStage();
	}

	if (event->getStage() == 1 && event->isActive())
	{
		event->getTarget()->handleEvent(event);
		if (event->isActive()) event->nextStage();
	}
	
	if (event->getStage() == 2 && event->isActive())
	{
		BOOST_FOREACH(EventListener *listener, postEventListeners)
		{
			if (event->getLastListener() != NULL)
			{
				if (event->getLastListener() != listener)
					continue;
				else
				{
					event->setLastListener(NULL);
					continue;
				}
			}

			listener->onEvent(event);
			if (!event->isActive()) 
			{
				event->setLastListener(listener);
				break;
			}
		}
		if (event->isActive()) event->nextStage();
	}
}

void World::addEventListener(EventListener *listener, bool pre, bool post)
{
	if (pre) preEventListeners.push_back(listener);
	if (post) postEventListeners.push_back(listener);
}

void World::removeEventListener(EventListener *listener)
{
}

void World::addMap(Map *map)
{
	map->setWorld(this);
	maps.push_back(map);
	ObjectCreateEvent *e = new ObjectCreateEvent(map);
	handleEvent(e);
}

void World::loadFromFile(std::string &path)
{
	using namespace luabind;

	try
	{
		LuaState *lua = lua_open();
		World::luaBind(lua);
		lua_dofile(lua, path.c_str());
		World *w = this;
		call_function<World *>(lua, "createWorld", boost::ref(*w));
		lua_close(lua);
		cout << "World::loadWorld: name=" << getName() << endl;
		cout << "World::loadWorld: desc=" << getDesc() << endl;
		BOOST_FOREACH(Map *map, getMaps())
		{
			printf("World::map: %dx%d\n", map->getWidth(), map->getHeight());
		}
	}
	catch (error &e)
	{
		object error_msg(from_stack(e.state(), -1));
		std::cout << "Lua error: " << error_msg << std::endl;
		exit(1);
	}
}
