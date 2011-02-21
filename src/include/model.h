#ifndef BASE_H
#define BASE_H

#include <boost/shared_ptr.hpp>
#include <boost/multi_array.hpp>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>

#include "include/util.h"

namespace Exile
{
namespace Model
{
	class Map;
	class Tile;
	class MapTile;
	class Entity;
	class Skill;
	class Progress;
	class Group;
	class Object;
	class Race;
	class Class;
	class NameGenerator;
	class World;
	class Event;
	typedef boost::shared_ptr<Event> EventPtr;
	typedef lua_State LuaState;

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
			virtual void addEventListener(EventListener *listener, bool pre = false, bool post = true) = 0;
			virtual void removeEventListener(EventListener *listener) = 0;
	};

	class Class
	{
		public:
			Class(std::string name) : name(name) { };
			static void luaBind(LuaState *lua);
		private:
			std::string name;
	};

	class Race
	{
		public:
			Race(std::string name) : name(name) { };
			static void luaBind(LuaState *lua);
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

	class Object : public EventHandler
	{
		public:
			Object();
			Object(const std::string &id);
			const std::string &getID() { return id; }
			MapTile *getLocation() { return location; }
			void setLocation(MapTile *mt) { location = mt; }

			void handleEvent(Event *event);

			static void luaBind(LuaState *lua);

			static const std::string getNewID();

		private:
			std::string id;
			int weight;
			MapTile *location;
			
			static int nextID;
	};
	
	class Entity : public Object
	{
		public:
			Entity();
			
			Race *getRace() { return race; }
			Class *getClass() { return cls; }

			static void luaBind(LuaState *lua);

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
			
			std::map<Skill *, Progress> skillMap;
			
			Group *group;
	};

	/**
	 * @class Skill
	 * Skill contains information about the skill but not information on 
	 * a particular entity's knowledge of the skill. For that see the
	 * @ref Entity::skillMap mapping.
	 **/
	class Skill
	{
		public:
			Skill();
			
		private:
			std::string name;
			std::string desc;
	};
	
	/**
	 * @class Progress
	 * Progress along a tiered path, such as skill levels.
	 **/
	class Progress
	{
		public:
		private:
	};
	
	/**
	 * @class Group
	 * A group is a group of entities whose decisions are controlled by
	 * a leader. If an entity is a member of two or more groups, their decisions
	 * are controlled by the leaders of those groups based on their
	 * leadership stat.
	 **/
	class Group
	{
		public:
			Group() { entities.clear(); }
			void setLeader(Model::Entity *entity) { leader = entity; };
			void addEntity(Model::Entity *entity) { entities.push_back(entity); };
			void removeEntity(Model::Entity *entity);
			const std::vector<Model::Entity*> &getEntities() { return entities; };
		
			static void luaBind(LuaState *lua);

		private:
			Model::Entity *leader;
			std::vector<Model::Entity *> entities;
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

			static void luaBind(LuaState *lua);
	
		private:
			std::string name;
			Terrain type;
			int variant;
	};
	
	class MapTile
	{
		public:
			MapTile(int i, int j, Tile *tile, Map *map);
			
			Tile *getTile() { return tile; }
			int getI() { return i; }
			int getJ() { return j; }
			Map *getMap() { return map; }
			
			void addObject(Object *object) { objects.push_back(object); }
			
			const std::vector<Object*> &getObjects() { return objects; }

			static void luaBind(LuaState *lua);
	
		private:
			int i, j;
			std::vector<Object*> objects;
			Tile *tile;
			Map *map;
	};

	typedef boost::multi_array<MapTile*, 2> MapTileArray;
	class Map : public Object
	{
		public:
			Map();
			Map(int w, int h);

			void generate();
			void addTile(Tile *tile) { tiles.push_back(tile); }
			
			const std::vector<Tile*> &getTiles() { return tiles; }
			MapTile *getTile(int i, int j);
			int getWidth() { return map->shape()[0]; }
			int getHeight() { return map->shape()[1]; }
			
			void setObjectLocation(Object *object, int i, int j);
			void setObjectLocation(Object *object, MapTile *tile);

			static void luaBind(LuaState *lua);
			
			void setWorld(World *w) { world = w; }
			
		private:
			World *world;
			std::vector<Tile *> tiles;
			MapTileArray *map;
			int width;
			int height;
	};

	/**
	 * @class Event
	 * Encapsulates discrete actions in the world so that they can be
	 * seemlessly passed between components (or across a remote connection).
	 * Each event is eventually dispatched to its target object. The dispatch
	 * may create other events to be handled, which will be labeled as child
	 * events for the purpose of rolling back/undoing.
	 **/
	class Event : public EventHandler
	{
		public:
			Event(Object *object);
			EventHandler *getEventHandler() { return rootHandler; }
			Object *getTarget() { return target; }

			void handleEvent(Event *event) { rootHandler->handleEvent(event); }
			
			void pause(EventListener *source);
			void resume();
			void cancel(EventListener *listener);
			
			bool isActive() { return active; }
			
			int getStage() { return stage; }
			void nextStage() { stage++; }
			void setLastListener(EventListener *listener) { lastListener = listener; }
			EventListener *getLastListener() { return lastListener; }
			
		private:
			bool active;
			int stage;
			EventListener *lastListener;
			
			EventPtr parent;
			Object *target;
			EventHandler *rootHandler;
	};

	/**
	 * @class ObjectCreateEvent
	 * Event that is fired when an object is created.
	 **/	
	class ObjectCreateEvent : public Event
	{
		public:
			ObjectCreateEvent(Object *object) : Event(object) { };
	};	

	class ObjectMoveEvent : public Event
	{
		public:
			ObjectMoveEvent(Object *object, MapTile *a, MapTile *b) : Event(object), src(a), dest(b) { };
	
			MapTile *getSrc() { return src; }
			MapTile *getDest() { return dest; }
		
		private:
			MapTile *src, *dest;
	};	
	
	/**
	 * @class TurnEvent
	 * Indicates that a turn has begun for a certain entity and specifies
	 * how many action points they may use during that turn.
	 **/
	class TurnEvent : public Event
	{
		public:
			TurnEvent(Entity *entity, int duration);
			
			int getDuration() { return duration; }
			Entity *getEntity() { return entity; }
		
		private:
			Entity *entity;
			int duration;
	};

	/**
	 * @class World
	 * Container for all objects, maps, entities, etc. for the instance
	 * currently being played. Also handles events (from the controller or
	 * children of other events) and dispatches them to their targets.
	 **/
	class World : public EventHandler, public EventListenable
	{
		public:
			World();
			
			const std::vector<Map*> &getMaps() { return maps; }
	
			void handleEvent(Event *event);
			void addEventListener(EventListener *listener, bool pre = false, bool post = true);
			void removeEventListener(EventListener *listener);
			
			static void luaBind(LuaState *lua);
			
			void loadFromFile(std::string &path);
			
			void addMap(Map *map);
			
			const std::string &getName() { return name; }
			const std::string &getDesc() { return desc; }
			
		private:
			std::string name;
			std::string desc;
			
			std::vector<Class *> classes;
			std::vector<Race *> races;
			
			std::vector<Tile *> tiles;
			std::vector<Map *> maps;
			
			std::vector<EventListener *> preEventListeners;
			std::vector<EventListener *> postEventListeners;
	};
}
}

#endif
