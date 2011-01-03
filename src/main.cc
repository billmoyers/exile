#include <Ogre.h>
#include <OgreErrorDialog.h>

#include "include/model.h"
#include "include/view.h"
#include "include/controller.h"

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <iostream>

using namespace std;

using namespace Exile::Model;
using namespace Exile::View;
using namespace Exile::Controller;

int	main( int argc, char **argv )
{
	Controller *c = new Controller("resources.cfg");
	c->run();
	return 0;
}
