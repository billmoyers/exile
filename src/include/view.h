#ifndef VIEW_H
#define VIEW_H

#include <Ogre.h>

#include "include/model.h"

namespace Exile
{
namespace View
{
	class ObjectView;
	typedef boost::shared_ptr<ObjectView> ObjectViewPtr;
	class MapView;
	typedef boost::shared_ptr<MapView> MapViewPtr;
	class WorldView;
	typedef boost::shared_ptr<WorldView> WorldViewPtr;
	class View;
	typedef boost::shared_ptr<View> ViewPtr;

	class ObjectView
	{
		public:
			ObjectView(Model::Object *object, Ogre::SceneNode *parentNode, WorldView *worldView);
			
		protected:
			void createMesh(std::string name);
		
		private:
			Model::Object *object;
			Ogre::SceneNode *node;
			Ogre::Entity *entity;
	};
	
	class MapView
	{
		public:
			MapView(Model::Map *map, WorldViewPtr worldView);
			
			Ogre::Vector3 getCenter();
			Model::Map *getMap() { return map; }
			Ogre::SceneNode *getSceneNode() { return node; }
			Ogre::SceneNode *getSceneNode(int i, int j);
			
		private:
			Model::Map *map;
			Ogre::SceneNode *node;
			WorldViewPtr worldView;
			
			boost::multi_array<Ogre::SceneNode *, 2> *sceneNodes;
	};
	
	class WorldView
	{
		public:
			WorldView(Model::World *world);
			
			Ogre::SceneManager *getSceneManager() { return sceneManager; }
			Model::World *getWorld() { return world; }
			MapViewPtr getMapView() { return mapView; }
		private:
			Ogre::SceneManager *sceneManager;
			Model::World *world;
			MapViewPtr mapView;
			
			std::map<Model::Object*, ObjectViewPtr> objectViews;
	};
	
	class View
	{
		public:
			View(Model::World *world);
			
			void update();
			
			
			Ogre::Camera *getCamera() { return camera; }
			static Ogre::RenderWindow *getWindow() { return window; }
	
		private:
			WorldViewPtr worldView;
			
			Ogre::Camera *camera;
			Ogre::Viewport *viewport;

			static Ogre::RenderWindow *window;
	};
}
}

#endif
