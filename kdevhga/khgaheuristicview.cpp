/*

	KHGAHeuristicView.cpp

	Window to follow the steps of an heuristic - Implementation.

	Copyright 1998-2004 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <rhga/robjh.h>
#include <rhga/rfirstnodeheuristic.h>
using namespace R;


#include <ginsth.h>
#include <gnodeinfos.h>
using namespace GALILEI;

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
// class MyNode
//
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
MyNode::MyNode(RNodesGA<MyNode,RObjH,GNodeInfosData,KHGAHeuristicView>* owner,unsigned id,GNodeInfosData* data)
	: RNodeGA<MyNode,RObjH,GNodeInfosData,KHGAHeuristicView>(owner,id,data)
{
}


//------------------------------------------------------------------------------
MyNode::MyNode(const MyNode* w)
	: RNodeGA<MyNode,RObjH,GNodeInfosData,KHGAHeuristicView>(w)
{
}


//------------------------------------------------------------------------------
int MyNode::Compare(const MyNode* n)
{
	return(Id-n->Id);
}



//-----------------------------------------------------------------------------
//
// class KHGAHeuristicView
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KHGAHeuristicView::KHGAHeuristicView(KDevHGADoc* pDoc,HeuristicType pType,QWidget *parent, const char *name,int wflags)
	: KDevHGAView(pDoc,parent,name,wflags), RNodesGA<MyNode,RObjH,GNodeInfosData,KHGAHeuristicView>(pDoc->Objs,pDoc->Objs->NbPtr),
	  Random(0), type(pType), Data(0), TreeHeur(0), Objs(0)
{
	Data=new GNodeInfosData(20);
	Init(Data);
	Objs=new RCursor<RObjH,unsigned int>();
	Objs->Set(pDoc->Objs);
	nbObjs = pDoc->Objs->NbPtr;
	draw=new QTreeInfos(pDoc,this);
	draw->setNodes(this);
	result=new QLabel(this);

	// Init the random generator
	Random = new RRandomGood(12345);

	// Init the heuristic
	step=theApp->step;

	switch(pType)
	{
		case FirstFit:
			TreeHeur = new RFirstNodeHeuristic<MyNode,RObjH,GNodeInfosData,KHGAHeuristicView>(Random,Objs,0);
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
		if(step)
		{
			draw->setNodes(this);
		}

		// test if the end
		if(TreeHeur->IsEnd())
		{
			TreeHeur->PostRun();
			result->setText("Done");
			draw->setNodes(this);
			emit endRun();
		}
		KApplication::kApplication()->processEvents(1000);
	}
	catch(RTreeHeuristicException& e)
    {
		KMessageBox::error(this,e.Msg.Latin1());
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
	if(Objs)
		delete Objs;
	if(Data)
		delete Data;
}
