#include "include/model.h"

using namespace Exile::Model;

Event::Event(Object *object)
{
	target = object;
	active = true;
	stage = 0;
	lastListener = NULL;
}

void Event::pause(EventListener *source)
{
	active = false;
	lastListener = source;
}

void Event::resume()
{
	active = true;
	rootHandler->handleEvent(this);
}

