#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <OIS.h>

#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>

#include "include/model.h"
#include "include/view.h"

namespace Exile
{
namespace Controller
{
	class Controller : 
		Model::ObjectListener,
		Ogre::WindowEventListener, 
		Ogre::FrameListener, 
		OIS::KeyListener, 
		OIS::MouseListener
	{
		public:
			Controller(std::string configFile);
			const std::vector<std::string> &listWorlds();
			Model::World *loadWorld(std::string &name);
			
			void onObjectCreated(Model::Object *object);
			void onObjectRemoved(Model::Object *object);
			
			int run();
		
		protected:
			bool keyPressed(const OIS::KeyEvent &arg);
			bool keyReleased(const OIS::KeyEvent &arg);
			
			bool mouseMoved(const OIS::MouseEvent &e);
			bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
			bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
			
			void windowResized(Ogre::RenderWindow* rw);
			void windowClosed(Ogre::RenderWindow* rw);			

			bool frameRenderingQueued(const Ogre::FrameEvent& evt);
			bool frameStarted(const Ogre::FrameEvent& evt);
			bool frameEnded(const Ogre::FrameEvent& evt);
			
		private:
			std::vector<std::string> worlds;
			
			Exile::View::ViewPtr view;
			
			OIS::InputManager *inputManager;
			OIS::Mouse *mouse;
			OIS::Keyboard *keyboard;
	};
	typedef boost::shared_ptr<Exile::Controller::Controller> ControllerPtr;

	class RemoteController : Controller
	{
	};
	typedef boost::shared_ptr<Exile::Controller::RemoteController> RemoteControllerPtr;
}
}

#endif
