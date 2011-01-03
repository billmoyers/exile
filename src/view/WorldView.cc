#include <boost/foreach.hpp>

#include "include/view.h"

using namespace Exile::View;
using namespace Exile::Model;

WorldView::WorldView(World *world) :
	world(world), mapView(NULL)
{
	using namespace Ogre;
	
	sceneManager = Ogre::Root::getSingleton().createSceneManager(
		Ogre::ST_GENERIC, "sceneManager");
	sceneManager->setAmbientLight(ColourValue(1, 1, 1));
}

void WorldView::onEvent(Event *event)
{
	printf("WorldView::onEvent\n");
	
	AS_BLOCK (ObjectCreateEvent *, oce, event)
	{
		AS_BLOCK (Map *, map, event->getTarget())
		{
			WorldViewPtr wvp(this);
			MapView *mapView = new MapView(map, wvp);
			mapViews[map] = mapView;
	
			this->mapView = mapView;
		} else {
			AS_BLOCK (Object *, object, event->getTarget())
			{
				MapTile *mtp = object->getLocation();
				int i = mtp->getI();
				int j = mtp->getJ();
				ObjectView *ov = new ObjectView(object, 
					mapView->getSceneNode(i, j), this);
				objectViews[object] = ov;
			}
		}
	}
	
	AS_BLOCK (ObjectMoveEvent *, ome, event)
	{
		AS_BLOCK (Object *, object, event->getTarget())
		{
			int i = ome->getDest()->getI();
			int j = ome->getDest()->getJ();
			if (objectViews.find(ome->getTarget()) == objectViews.end())
			{
				MapTile *mtp = object->getLocation();
				int i = mtp->getI();
				int j = mtp->getJ();
				ObjectView *ov = new ObjectView(object, 
					mapView->getSceneNode(i, j), this);
				objectViews[object] = ov;
			}
			objectViews[ome->getTarget()]->setParentNode(mapView->getSceneNode(i, j));
		}
	}
}
