/*

  khgaheuristicview.cpp

  Description - Implementation.

  (c) 2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <time.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rhga/rfirstnodeheuristic.h>


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <klocale.h>
#include <kmessagebox.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdevhga.h"
#include "khgaheuristicview.h"
#include "kdevhgadoc.h"



//-----------------------------------------------------------------------------
//
// class KHGAHeuristicView
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KHGAHeuristicView::KHGAHeuristicView(KDevHGADoc* pDoc,HeuristicType pType,QWidget *parent, const char *name,int wflags)
	: KDevHGAView(pDoc,parent,name,wflags), RNodesGA<RNodeWords,RObjH,RNodeWordsData>(pDoc->Objs->NbPtr,pDoc->Objs),
	  Random(0), type(pType), TreeHeur(0), Data(0)
{
	Data=new RNodeWordsData(20);
	Init(Data);
	nbObjs = pDoc->Objs->NbPtr;
	draw=new QListWords(pDoc,this);
	result=new QLabel(this);

	// Init the random generator
	Random = new RRandomGood(12345);

	// Init the heuristic
	step=theApp->step;

	switch(pType)
	{
		case FirstFit:
			TreeHeur = new RFirstNodeHeuristic<RNodeWords,RObjH,RNodeWordsData>(Random,pDoc->Objs);
			break;
	}
	TreeHeur->Init(this);
	connect(this,SIGNAL(endRun()),theApp,SLOT(slotEndHeuristic(void)));
}


//-----------------------------------------------------------------------------
void KHGAHeuristicView::setTitle(QString _title)
{
	time_t now;
	static char today[30];
	struct tm *l_time;

	switch(type)
	{
		case FirstFit:
			_title="First-Fit Heuristic: "+_title;
			break;
	}
	now=time((time_t *)0);
	l_time = localtime(&now);
	sprintf(today," (%u-%u-%u %u:%u:%u)",l_time->tm_year+1900,l_time->tm_mon+1,l_time->tm_mday,l_time->tm_hour,l_time->tm_min,l_time->tm_sec);
	setCaption(_title+today);
}


//-----------------------------------------------------------------------------
void KHGAHeuristicView::resizeEvent(QResizeEvent *)
{
	result->resize(width(),result->height());
	result->move(0,height()-result->height());
	draw->resize(width(),height()-result->height());
}


//-----------------------------------------------------------------------------
void KHGAHeuristicView::RunHeuristic(void)
{
	Stop=false;
	// Run the heuristic
	if(step)
		NextStep();
	else
		while((!TreeHeur->IsEnd())&&(!Stop))
			NextStep();
}


//-----------------------------------------------------------------------------
void KHGAHeuristicView::NextStep(void)
{
	try
	{
		TreeHeur->PutNextObject();
//		if(step)
//		{
//			draw->addInfo(CurInfo);
//			while(free->NbPtr>nbFree)
//				addFree(free->Tab[nbFree++]);
//		}

		// test if the end
		if(TreeHeur->IsEnd())
		{
			TreeHeur->PostRun();
			result->setText("Done");
			Verify();
			draw->setNodes(this);
			emit endRun();
		}
		KApplication::kApplication()->processEvents(1000);
	}
	//catch(RTreeHeuristicException& e)
	catch(...)
    {
		//KMessageBox::error(this,QString(e.Msg));
		KMessageBox::error(this,"Error in GA");
		Stop=true;
    }
}


//-----------------------------------------------------------------------------
void KHGAHeuristicView::RunToEnd(void)
{
	step=false;
	while(!TreeHeur->IsEnd())
	{
		NextStep();
	}
}


//-----------------------------------------------------------------------------
KHGAHeuristicView::~KHGAHeuristicView()
{
	if(TreeHeur)
		delete TreeHeur;
	if(Random)
		delete Random;
}
