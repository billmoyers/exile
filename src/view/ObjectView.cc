#include <boost/foreach.hpp>

#include "include/view.h"

using namespace std;
using namespace Exile::View;
using namespace Exile::Model;

ObjectView::ObjectView(Model::Object *object, Ogre::SceneNode *node, WorldView *worldView) :
	object(object), node(node)
{
	using namespace Ogre;
	
	string name;

	switch (rand()%3)
	{
		case 0:
			name = "entity/knight-human-male";
			break;
		case 1:
			name = "entity/knight-human-female";
			break;
		case 2:
			name = "entity/noble-human-male";
			break;			
	}

	createMesh(name);
	
	char buf[50];
	sprintf(buf, "%d", rand()%50);

	Ogre::Entity *entity = NULL;
	
	entity = worldView->getSceneManager()->createEntity(name+buf,
		name+".mesh");
	entity->setMaterialName(name);

	Vector3 centerOffset(0, 0, 1.0);

	string dataName(name+".data");
	ResourceGroupManager &rgm = ResourceGroupManager::getSingleton();

	if (rgm.resourceExists("General", dataName))
	{
		ConfigFile dataFile;
		dataFile.loadFromResourceSystem(dataName, "General");
		string centerStr(dataFile.getSetting("center"));
		int x, y;
		if (sscanf(centerStr.c_str(), "%d,%d", &x, &y) == 2)
		{
			string dimStr(dataFile.getSetting("dim"));
			int w, h;
			if (sscanf(dimStr.c_str(), "%d,%d", &w, &h) == 2)
			{
				double uw = 3.0*w/512.0;
				double uh = 3.0*h/512.0;
				
				double cx = uw*(x/(float)w);
				double cy = uh*((h-y)/(float)h);
				
				double ox = uw/2;
				double oy = uh/2;

				centerOffset[0] = ox-cx;
				centerOffset[1] = oy-cy;
			}
		}
	}
	
	Ogre::SceneNode *tnode = node->createChildSceneNode(name+buf, centerOffset);
	tnode->attachObject(entity);
	this->node = tnode;
}

void ObjectView::createMesh(string name)
{
	using namespace Ogre;

	MeshManager &mm = MeshManager::getSingleton();
	
	string meshName(name);
	meshName += ".mesh";
	
	if (mm.resourceExists(meshName))
		return;

	string imageName(name);
	imageName += ".png";
	Image image;
	image.load(imageName, "General");
	double w = image.getWidth();
	double h = image.getHeight();
	double ow = w;
	double oh = h;
	
	w /= 512;
	h /= 512;
	
	w *= 3;
	h *= 3;
		
	mm.createPlane(meshName, "General",
		Plane(Vector3::UNIT_Z, Vector3::ZERO), w, h, 1, 1);
}

void ObjectView::setParentNode(Ogre::SceneNode *parentNode)
{
	node->getParentSceneNode()->removeChild(node);
	parentNode->addChild(node);
}
