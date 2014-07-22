/*

	KHeuristic.cpp

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
#include <kdevhga.h>
#include <kheuristic.h>



//-----------------------------------------------------------------------------
//
// class KHeuristic
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KHeuristic::KHeuristic(KPrjView* project)
	: QMdiSubWindow(), Ui_KHeuristic(), Project(project), Inst(0,1,project->GetObjs(),"FirstFit",0)
{
	// Set the widget part
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	Tree->setProblem(project);
	Result->setText("Running...");

	Inst.Init();

	// Init the heuristic
	step=theApp->MustStep();
	Inst.GetHeuristic(0)->Init(Inst.Chromosomes[0]);
	connect(this,SIGNAL(endRun()),theApp,SLOT(slotEndHeuristic(void)));

	time_t now;
	static char today[30];
	struct tm *l_time;

	now=time((time_t *)0);
	l_time = localtime(&now);
	sprintf(today," (%u-%u-%u %u:%u:%u)",l_time->tm_year+1900,l_time->tm_mon+1,l_time->tm_mday,l_time->tm_hour,l_time->tm_min,l_time->tm_sec);
	setWindowTitle(ToQString("First Node: "+Project->GetURI().GetPath()+today));
}


//-----------------------------------------------------------------------------
void KHeuristic::RunHeuristic(void)
{
	Stop=false;

	// Run the heuristic
	if(step)
		NextStep(false);
	else
		while((!Inst.GetHeuristic(0)->IsEnd())&&(!Stop))
		{
			//cout<<"Avant : Nb Top Nodes: "<<Inst.Chromosomes[0]->GetNbTopNodes()<<endl;
			NextStep(false);
			//cout<<"Après : Nb Top Nodes: "<<Inst.Chromosomes[0]->GetNbTopNodes()<<endl;
		}
	Tree->setNodes(Inst.Chromosomes[0]);
}


//-----------------------------------------------------------------------------
void KHeuristic::NextStep(bool paint)
{
	try
	{
		Inst.GetHeuristic(0)->PutNextObject();

		// test if the end
		if(Inst.GetHeuristic(0)->IsEnd())
		{
			Inst.GetHeuristic(0)->PostRun();
			Result->setText("Done");
			if(paint)
				Tree->setNodes(Inst.Chromosomes[0]);
			emit endRun();
		}
		else
		{
			if(paint)
				Tree->setNodes(Inst.Chromosomes[0]);
		}
	}
	catch(RException& e)
   {
		KMessageBox::error(this,ToQString(e.GetMsg()));
		Stop=true;
   }
}


//-----------------------------------------------------------------------------
void KHeuristic::RunToEnd(void)
{
	step=false;
	while(!Inst.GetHeuristic(0)->IsEnd())
	{
		NextStep();
	}
}


//-----------------------------------------------------------------------------
KHeuristic::~KHeuristic(void)
{
	Project->remove(this);
}
