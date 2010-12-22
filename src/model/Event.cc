#include "include/model.h"

using namespace Exile::Model;

Event::Event(Object *object) :
	dispatched(false), active(true)
{
	target = object;
}

void Event::cancel(EventListener *listener)
{
	active = false;
	lastListener = listener;
}

void Event::resume()
{
	active = true;
	rootHandler->handleEvent(event);
}

