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
	class ObjectListener;

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

	class Object
	{
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
	class Map
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

	class ObjectListener
	{
		public:
			virtual void onObjectCreated(Object *object) = 0;
			virtual void onObjectRemoved(Object *object) = 0;
	};
	
	class World
	{
		public:
			World();
			
			const std::vector<Map*> &getMaps() { return maps; }
	
			void addObjectListener(ObjectListener *listener) { objectListeners.push_back(listener); }
			void removeObjectListener(ObjectListener *listener);
	
		private:
			std::vector<Class *> classes;
			std::vector<Race *> races;
			
			std::vector<Tile *> tiles;
			std::vector<Map *> maps;
			
			std::vector<ObjectListener *> objectListeners;
	};
}
}

#endif
