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
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>



//-----------------------------------------------------------------------------
// include files for current project
#include <rexception.h>
#include "kdevhga.h"


//-----------------------------------------------------------------------------
static const char *description =
	I18N_NOOP("KDevHGA\nDevelopment Application for HGA");


//-----------------------------------------------------------------------------
KDevHGA* theApp;


//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE,"");

    // Information about the application
	KAboutData aboutData("kdevhga",0,ki18n("KDevHGA"),"1.89",ki18n(description),
			KAboutData::License_GPL,ki18n("(C) 1998-2014 by Pascal Francq\n"),
			KLocalizedString(),"http://www.otlet-institute.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Project Manager"),"pascal@francq.info");

	// Init
	KCmdLineArgs::init( argc, argv, &aboutData );
   KCmdLineOptions options;
	KCmdLineArgs::addCmdLineOptions(options);


	try
	{
		KApplication app;
		theApp=new KDevHGA(argc,argv);
		if(app.isSessionRestored())
		{
//			RESTORE(theApp);
		}
		else
		{
			theApp->show();
		}
		return(app.exec());
	}
	catch(R::RException& e)
	{
		std::cout<<e.GetMsg()<<std::endl;
	}
	catch(std::exception& e)
	{
		std::cout<<e.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Unknown problem"<<std::endl;
	}
 	return(0);
}
