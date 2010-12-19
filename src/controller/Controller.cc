#include <iostream>

#include <boost/foreach.hpp>

#include "include/controller.h"

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
		}
	}

	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
	cout << "Controller: created." << endl;
}

const std::vector<std::string> &Controller::listWorlds()
{
	using namespace Ogre;

	worlds.clear();

	StringVectorPtr files = 		
		ResourceGroupManager::getSingleton().findResourceNames(
			"General", "*.world.xml", false);

	cout << __FUNCTION__ << ": " << files->size() << " worlds found" << endl;

	BOOST_FOREACH(string s, *files)		
	{
		worlds.push_back(s);
	}		
	
	return worlds;
}

World *Controller::loadWorld(std::string &name)
{
	using namespace Ogre;

	DataStreamPtr ds = 
		ResourceGroupManager::getSingleton().openResource(
			"General", name);

	World *world = new World();
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
	World *world = new World();
	ViewPtr newView(new Exile::View::View(world));
	view = newView;
	
	world->addEventListener(this);

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
