/*

	KHGAHeuristicView.cpp

	Window to follow the steps of an heuristic - Implementation.

	Copyright 1998-2008 by the Université Libre de Bruxelles.

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
#include <robjh.h>
#include <rfirstnodeheuristic.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ginsth.h>
#include <gnodeinfos.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
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
KHGAHeuristicView::KHGAHeuristicView(KDevHGADoc* pDoc,const RString& pType,QWidget *parent, const char *name,int wflags)
	: KDevHGAView(pDoc,parent,name,wflags), type(pType), Inst(0,1,*pDoc->Objs,"FirstFit",0), Objs(*pDoc->Objs)
{
	Inst.Init();
	nbObjs = pDoc->Objs->GetNb();
	draw=new QTreeInfos(pDoc,this);
	draw->setNodes(Inst.Chromosomes[0]);
	result=new QLabel(this);

	// Init the heuristic
	step=theApp->step;
	Inst.GetHeuristic(0)->Init(Inst.Chromosomes[0]);
	connect(this,SIGNAL(endRun()),theApp,SLOT(slotEndHeuristic(void)));
}


//-----------------------------------------------------------------------------
void KHGAHeuristicView::setTitle(QString _title)
{
	time_t now;
	static char today[30];
	struct tm *l_time;

	if(type=="FirstFit")
		_title="First-Fit Heuristic: "+_title;
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
		while((!Inst.GetHeuristic(0)->IsEnd())&&(!Stop))
			NextStep();
}


//-----------------------------------------------------------------------------
void KHGAHeuristicView::NextStep(void)
{
	try
	{
		Inst.GetHeuristic(0)->PutNextObject();

		// test if the end
		if(Inst.GetHeuristic(0)->IsEnd())
		{
			Inst.GetHeuristic(0)->PostRun();
			result->setText("Done");
			draw->setNodes(Inst.Chromosomes[0]);
			emit endRun();
		}
		else
		{
			draw->setNodes(Inst.Chromosomes[0]);
		}
		KApplication::kApplication()->processEvents(1000);
	}
	catch(RException& e)
    {
		KMessageBox::error(this,e.GetMsg());
		Stop=true;
    }
}


//-----------------------------------------------------------------------------
void KHGAHeuristicView::RunToEnd(void)
{
	step=false;
	while(!Inst.GetHeuristic(0)->IsEnd())
	{
		NextStep();
	}
}


//-----------------------------------------------------------------------------
KHGAHeuristicView::~KHGAHeuristicView(void)
{
}
