#include "include/view.h"

using namespace Exile::View;
using namespace Exile::Model;
using namespace std;

MapView::MapView(Map *map, WorldViewPtr worldView) :
	map(map), worldView(worldView)
{
	using namespace Ogre;

	Ogre::MeshManager::getSingleton().createPlane("plane.mesh", "General",
		Plane(Vector3::UNIT_Z, Vector3::ZERO), 1, 1, 1, 1);

	hover = NULL;
	node = worldView->getSceneManager()->getRootSceneNode()->createChildSceneNode("map", Vector3::ZERO);
	
	int w = map->getWidth();
	int h = map->getHeight();
	
	sceneNodes = new boost::multi_array<Ogre::SceneNode *, 2>(boost::extents[w][h]);

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			string s("tile");
			s += i;
			s += j;
			
			Ogre::Entity *entity = worldView->getSceneManager()->createEntity(s, "plane.mesh");
			entity->setMaterialName("terrain/" + map->getTile(i, j)->getTile()->getName());

			Ogre::SceneNode *tnode = node->createChildSceneNode(s, Vector3(i, j, 0.1*(h-j)));
			tnode->attachObject(entity);
			
			(*sceneNodes)[i][j] = tnode;
		}
	}
}

Ogre::Vector3 MapView::getCenter()
{
	return Ogre::Vector3(map->getWidth()/2, map->getHeight()/2, 0);
}

Ogre::SceneNode *MapView::getSceneNode(int i, int j)
{
	return (*sceneNodes)[i][j];
}

MapTile *MapView::getTileAt(Ogre::Vector2 &pos)
{
	return map->getTile((int) (pos[0]+0.5), (int) (pos[1]+0.5));
}

void MapView::hoverTile(Model::MapTile *tile)
{
	using namespace Ogre;

	if (hover != NULL && hover != tile)
	{
		SceneNode *hn = getSceneNode(hover->getI(), hover->getJ());
		if (hn != NULL)
			for (int i = 0; i < hn->numChildren(); i++)
				hn->getChild(i)->setScale(1, 1, 1);
	}
	if (hover != tile)
	{
		SceneNode *tn = getSceneNode(tile->getI(), tile->getJ());
		if (tn != NULL)
			for (int i = 0; i < tn->numChildren(); i++)
				tn->getChild(i)->setScale(1.2, 1.2, 1);
	}
	hover = tile;
}
