#include <iostream>

#include <Ogre.h>

#include "include/view.h"
#include "include/model.h"

using namespace std;
using namespace Exile::View;
using namespace Exile::Model;

Ogre::RenderWindow *View::window;

View::View(World *world)
{
	using namespace Ogre;
	
	if (View::window == NULL)
	{
		Root::getSingleton().restoreConfig() || Root::getSingleton().showConfigDialog();
		View::window = Root::getSingleton().initialise(true, "Window");
	}

	WorldViewPtr wvp(new WorldView(world));
	worldView = wvp;

	camera = worldView->getSceneManager()->createCamera("camera");
	camera->setProjectionType(PT_ORTHOGRAPHIC);
	camera->setPosition(worldView->getMapView()->getCenter() + Vector3(0, 0, 10));
	camera->lookAt(worldView->getMapView()->getCenter());
	camera->setOrthoWindowWidth(20);
	
	viewport = View::window->addViewport(camera);
	viewport->setBackgroundColour(ColourValue(0, 0, 0));
	
	camera->setNearClipDistance(0.5);
	camera->setFarClipDistance(5000);		
}
