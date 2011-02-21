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
			
			void setLocation(Ogre::Vector2 loc);
			void setParentNode(Ogre::SceneNode *parentNode);
			
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
			Model::MapTile *getTileAt(Ogre::Vector2 &pos);
			
			void hoverTile(Model::MapTile *tile);

		private:
			Model::Map *map;
			Ogre::SceneNode *node;
			WorldViewPtr worldView;
			Model::MapTile *hover;
			
			boost::multi_array<Ogre::SceneNode *, 2> *sceneNodes;
	};
	
	class WorldView : public Model::EventListener
	{
		public:
			WorldView(Model::World *world);

			void onEvent(Model::Event *event);
			
			Ogre::SceneManager *getSceneManager() { return sceneManager; }
			Model::World *getWorld() { return world; }
			MapView *getMapView() { return mapView; }
		private:
			Ogre::SceneManager *sceneManager;
			Model::World *world;

			MapView *mapView;			
			std::map<Model::Map*, MapView*> mapViews;
			std::map<Model::Object*, ObjectView*> objectViews;
	};
	
	class View : public Model::EventListener
	{
		public:
			View(WorldView *worldView);
			
			void update();
			WorldView *getWorldView() { return worldView; }
			MapView *getMapView() { return worldView->getMapView(); }
			
			void onEvent(Model::Event *event);
			
			Ogre::Camera *getCamera() { return camera; }
			static Ogre::RenderWindow *getWindow() { return window; }
	
		private:
			WorldView *worldView;
			
			Ogre::Camera *camera;
			Ogre::Viewport *viewport;

			static Ogre::RenderWindow *window;
	};
}
}

#endif
