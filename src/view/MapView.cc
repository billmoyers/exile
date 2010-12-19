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
