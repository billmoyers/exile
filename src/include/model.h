#ifndef BASE_H
#define BASE_H

#include <boost/shared_ptr.hpp>
#include <boost/multi_array.hpp>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>

namespace Exile
{
namespace Model
{
	class Map;
	class Tile;
	class MapTile;
	class Entity;
	class Object;
	class Race;
	class Class;
	class NameGenerator;
	class World;
	class Event;
	typedef boost::shared_ptr<Event> EventPtr;

	class EventListener
	{
		public:
			virtual void onEvent(Event *event) = 0;
	};	
	
	/**
	 * @class EventHandler
	 * Instances of this type can either dispatch events to lower level
	 * handlers or perform the actual work of the event.
	 **/
	class EventHandler
	{
		public:
			virtual void handleEvent(Event *event) = 0;
	};
	
	/**
	 * @class EventListenable
	 * If instances of this type can receive events, @ref EventListener
	 * objects should be able to register to listen to those events.
	 **/
	class EventListenable
	{
		public:
			virtual void addEventListener(EventListener *listener) = 0;
			virtual void removeEventListener(EventListener *listener) = 0;
	};

	class Class
	{
		public:
			Class(std::string name) : name(name) { };
		private:
			std::string name;
	};

	class Race
	{
		public:
			Race(std::string name) : name(name) { };
		private:
			std::string name;
	};

	class NameGenerator
	{
		public:
			std::string generateName();
			
		private:
			std::vector<std::string> nouns;
			std::vector<std::string> adjectives;
			std::vector<std::string> verbs;
	};

	class Object : EventHandler
	{
		public:
			void handleEvent(Event *event);
			
		private:
			int weight;
			Tile *location;
	};
	
	class Entity : Object
	{
		public:
			Entity(Race *race, Class *cls);
			
			Race *getRace() { return race; }
			Class *getClass() { return cls; }

		private:
			std::string name;
			std::string preTitle;
			std::string postTitle;
		
			Race *race;
			Class *cls;
			
			int hp;
			int mp;
			int ap;
			
			int strength;
			int dexterity;
			int intelligence;
			int wisdom;
			int charisma;
			int leadership;
	};

	enum Terrain
	{
		VOID,
		WALL,
		GRASS,
		WATER,
		FOREST,
		FLOOR,
		PAVED,
		SNOW,
		MUD
	};

	class Tile
	{
		public:
			Tile() { };
			Tile(std::string name, Terrain type, int variant) :
				name(name), type(type), variant(variant) { };
	
			const std::string &getName() { return name; }
	
		private:
			std::string name;
			Terrain type;
			int variant;
	};
	
	class MapTile
	{
		public:
			MapTile();
			MapTile(Tile *tile);
			
			Tile *getTile() { return tile; }
			
			void addObject(Object *object) { objects.push_back(object); }
			
			const std::vector<Object*> &getObjects() { return objects; }
	
		private:
			std::vector<Object*> objects;
			Tile *tile;
	};

	typedef boost::multi_array<MapTile*, 2> MapTileArray;
	class Map : Object
	{
		public:
			Map();
			Map(std::vector<Tile*> tiles, int w, int h, World *world);
			
			const std::vector<Tile*> &getTiles() { return tiles; }
			MapTile *getTile(int i, int j) { return (*map)[i][j]; }
			int getWidth() { return map->shape()[0]; }
			int getHeight() { return map->shape()[1]; }
			
			void setObjectLocation(Object *object, int i, int j);
			
		private:
			World *world;
			std::vector<Tile *> tiles;
			MapTileArray *map;
	};

	/**
	 * @class Event
	 * Encapsulates discrete actions in the world so that they can be
	 * seemlessly passed between components (or across a remote connection).
	 * Each event is eventually dispatched to its target object. The dispatch
	 * may create other events to be handled, which will be labeled as child
	 * events for the purpose of rolling back/undoing.
	 **/
	class Event : EventHandler
	{
		public:
			EventHandler *getEventHandler() { return rootHandler; }
			Object *getTarget() { return target; }

			void handleEvent(Event *event) { rootHandler->handleEvent(event); }
			
		private:
			EventPtr parent;
			Object *target;
			EventHandler *rootHandler;
	};

	/**
	 * @class ObjectCreateEvent
	 * Event that is fired when an object is created.
	 **/	
	class ObjectCreateEvent : Event
	{
		public:
			ObjectCreateEvent(Object *object);
	};	

	/**
	 * @class World
	 * Container for all objects, maps, entities, etc. for the instance
	 * currently being played. Also handles events (from the controller or
	 * children of other events) and dispatches them to their targets.
	 **/
	class World : EventHandler, EventListenable
	{
		public:
			World();
			
			const std::vector<Map*> &getMaps() { return maps; }
	
			void handleEvent(Event *event);
			void addEventListener(EventListener *listener) { eventListeners.push_back(listener); }
			void removeEventListener(EventListener *listener);
			
		private:
			std::vector<Class *> classes;
			std::vector<Race *> races;
			
			std::vector<Tile *> tiles;
			std::vector<Map *> maps;
			
			std::vector<EventListener *> eventListeners;
	};
}
}

#endif
