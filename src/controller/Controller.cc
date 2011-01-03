#include <iostream>

#include <boost/foreach.hpp>

#include "include/controller.h"

#include <boost/algorithm/string.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/nvp.hpp>

#include <OIS/OIS.h>

using namespace Exile::Model;
using namespace Exile::Controller;
using namespace Exile::View;
using namespace std;

Controller::Controller(std::string configFile)
{
	using namespace Ogre;

	Root *root = new Root("plugins.cfg", "ogre.cfg", "ogre.log");

	ConfigFile cf;
	cf.load(configFile);
	ConfigFile::SectionIterator section = cf.getSectionIterator();

	while (section.hasMoreElements())
	{
		String section_name = section.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = section.getNext();
		for (ConfigFile::SettingsMultiMap::iterator i = settings->begin();
				i != settings->end(); ++i)
		{
			String type_name = i->first;
			String arch_name = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(
					arch_name, type_name, section_name);
			printf("addResourceLocation: %s %s %s\n",
				arch_name.c_str(),
				type_name.c_str(),
				section_name.c_str());
		}
	}

	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
	cout << "Controller: created." << endl;

	activeEntity = NULL;
}

const std::vector<std::string> &Controller::listWorlds()
{
	using namespace Ogre;

	worlds.clear();

	StringVectorPtr files = 		
		ResourceGroupManager::getSingleton().findResourceNames(
			"General", "worlds/*", true);

	BOOST_FOREACH(string s, *files)		
	{
		std::vector<std::string> strs;
		boost::split(strs, s, boost::is_any_of("/\\"));
		worlds.push_back(strs[1]);
	}		
	
	return worlds;
}

World *Controller::loadWorld(std::string &name)
{
	using namespace Ogre;

	printf("Controller::loadWorld: name=%s\n", name.c_str());

	ResourceGroupManager::getSingleton().addResourceLocation(
		"../media/worlds/"+name, "FileSystem", "World::"+name);
	
	FileInfoListPtr fileInfoList = ResourceGroupManager::getSingleton().
		listResourceFileInfo("World::"+name);	
		
	BOOST_FOREACH(FileInfo info, *fileInfoList)
	{
		if (info.filename == "world.lua")
		{
			string path("../media/worlds/"+name+"/world.lua");
			world->loadFromFile(path);
			return world;
		}
	}
	
	printf("Controller::loadWorld: Failed finding appropriate resources\n");
	
	return world;
}
			
bool Controller::keyPressed(const OIS::KeyEvent &arg)
{
	cout << "Controller::keyPressed" << endl;
	if (arg.key == OIS::KC_ESCAPE)
		exit(1);
		
	//CEGUI::System::getSingleton().injectKeyDown( arg.key );
	return true;
}

bool Controller::keyReleased(const OIS::KeyEvent &arg)
{
	cout << "Controller::keyReleased" << endl;
	return true;
}

bool Controller::mouseMoved(const OIS::MouseEvent &e)
{	
}

bool Controller::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	using namespace Ogre;

	if (id == OIS::MB_Left)
	{
		double uw = view->getCamera()->getOrthoWindowWidth();
		double uh = uw;
		
		double sw = view->getWindow()->getWidth();
		double sh = view->getWindow()->getHeight();

		double wx = e.state.X.abs;
		double wy = ((sw-sh)/2) + sh - e.state.Y.abs;

		double cx = uw*(wx/sw) - uw/2;
		double cy = uh*(wy/sw) - uh/2;

		Vector2 clickPos(cx, cy);
		Vector2 cameraPos(view->getCamera()->getPosition()[0], view->getCamera()->getPosition()[1]);
		Vector2 mapPos(0, 0);
		Vector2 fpos = clickPos + cameraPos - mapPos;
		
		MapTile *m = view->getMapView()->getTileAt(fpos);

		printf("Controller::click (%f,%f)\n", fpos[0], fpos[1]);

		if (m != NULL)
		{
			printf("Controller::click: tile=%d,%d\n", m->getI(), m->getJ());

			bool selected = false;
			
			BOOST_FOREACH(Object *object, m->getObjects())
			{
				Model::Entity *e = dynamic_cast<Model::Entity *>(object);
				if (e != NULL)
				{
					activeEntity = e;
					selected = true;
				}
			}

			if (!selected)
			{
				if (activeEntity != NULL)
				{
					Event *event = new ObjectMoveEvent(activeEntity, activeEntity->getLocation(), m);
					world->handleEvent(event);
				}
			}
		}
	}
}

bool Controller::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void Controller::windowResized(Ogre::RenderWindow* rw)
{
	cout << "Controller::windowResized" << endl;
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);
			 
	const OIS::MouseState &ms = mouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void Controller::windowClosed(Ogre::RenderWindow* rw)
{
	cout << "Controller::windowClosed" << endl;
	inputManager->destroyInputObject(mouse);
	inputManager->destroyInputObject(keyboard);
						 
	OIS::InputManager::destroyInputSystem(inputManager);
	inputManager = NULL;
}

bool Controller::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (View::View::getWindow()->isClosed())
		return false;

	keyboard->capture();
	mouse->capture();
	
	if (keyboard->isKeyDown(OIS::KC_UP))
		view->getCamera()->move(Ogre::Vector3(0, 0.1, 0));
	if (keyboard->isKeyDown(OIS::KC_DOWN))
		view->getCamera()->move(Ogre::Vector3(0, -0.1, 0));
	if (keyboard->isKeyDown(OIS::KC_LEFT))
		view->getCamera()->move(Ogre::Vector3(-0.1, 0, 0));
	if (keyboard->isKeyDown(OIS::KC_RIGHT))
		view->getCamera()->move(Ogre::Vector3(0.1, 0, 0));
	
	return true;					
}

int Controller::run()
{
	world = new World();
	WorldView *worldView = new WorldView(world);
	view = new View::View(worldView);

	world->addEventListener(this);
	world->addEventListener(worldView);
	world->addEventListener(view);

	std::string worldName(listWorlds()[0]);
	loadWorld(worldName);
	
	size_t hWnd = 0;
	View::View::getWindow()->getCustomAttribute("WINDOW", &hWnd);
	std::stringstream wnd;
	wnd << hWnd;
	
	OIS::ParamList paramList;

	paramList.insert(std::make_pair(std::string("WINDOW"), wnd.str()));
	paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
	
	inputManager = OIS::InputManager::createInputSystem(paramList);
	
	mouse = static_cast<OIS::Mouse*>
		(inputManager->createInputObject(OIS::OISMouse, true));
	keyboard = static_cast<OIS::Keyboard*>
		(inputManager->createInputObject(OIS::OISKeyboard, true));
	
	keyboard->setEventCallback(this);
	mouse->setEventCallback(this);

	unsigned int width, height, depth;
	int left, top;
	View::View::getWindow()->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mouse->getMouseState();
	ms.width = width;
	ms.height = height;

	Ogre::WindowEventUtilities::addWindowEventListener(View::View::getWindow(), this);
	Ogre::Root::getSingleton().addFrameListener(this);
	Ogre::Root::getSingleton().startRendering();
}

void Controller::onEvent(Event *event)
{
}

bool Controller::frameStarted(const Ogre::FrameEvent& evt)
{
	return frameRenderingQueued(evt);
}

bool Controller::frameEnded(const Ogre::FrameEvent& evt)
{
}
