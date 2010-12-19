#include <boost/foreach.hpp>

#include "include/view.h"

using namespace Exile::View;
using namespace Exile::Model;

WorldView::WorldView(World *world) :
	world(world)
{
	using namespace Ogre;
	
	sceneManager = Ogre::Root::getSingleton().createSceneManager(
		Ogre::ST_GENERIC, "sceneManager");
	sceneManager->setAmbientLight(ColourValue(1, 1, 1));
	
	WorldViewPtr wvp(this);
	MapViewPtr mvp(new MapView(world->getMaps()[0], wvp));
	mapView = mvp;
	
	for (int i = 0; i < mapView->getMap()->getWidth(); i++)
	{
		for (int j = 0; j < mapView->getMap()->getHeight(); j++)
		{
			MapTile *mtp = mapView->getMap()->getTile(i, j);
			BOOST_FOREACH(Object *object, mtp->getObjects())
			{
				if (objectViews.find(object) == objectViews.end())
				{
					ObjectView *ov = new ObjectView(object, mvp->getSceneNode(i, j), this);
					ObjectViewPtr ovp(ov);
					objectViews[object] = ovp;;
				}
			}
		}
	}
}
