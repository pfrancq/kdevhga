/*

	main.cpp

	Description - Implementation.

	Copyright 1998-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for KDE
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>


//-----------------------------------------------------------------------------
// include files for current project
#include "kdevhga.h"


//-----------------------------------------------------------------------------
static const char *description =
	I18N_NOOP("KDevHGA\nDevelopment Application for HGA");


//-----------------------------------------------------------------------------
static KCmdLineOptions options[] =
{
  { "+[File]", I18N_NOOP("file to open"), 0 },
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};


//-----------------------------------------------------------------------------
KDevHGAApp* theApp;


//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	KAboutData aboutData( "kdevhga", I18N_NOOP("KDevHGA"),
		VERSION, description, KAboutData::License_GPL,
		"(c) 1998-2002, Université Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Project Manager"), "pfrancq@ulb.ac.be");

	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

	try
	{
		KApplication app;

		if(app.isRestored())
		{
			RESTORE(KDevHGAApp);
		}
		else
		{
			theApp = new KDevHGAApp();
			theApp->show();
		}
		return app.exec();
	}
	catch(...)
	{
		std::cout<<"Problem"<<std::endl;
	}
 	return(0);
}
