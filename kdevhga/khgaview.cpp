/*

	KHGAView.h

	HGA Window - Implementation.

	Copyright 1998-2014 by Pascal Francq (pascal@francq.info).

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
// include files for GALILEI
#include <ginsth.h>
#include <gchromoh.h>
#include <rqt.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QKeyEvent>
#include <klocale.h>
#include <kmessagebox.h>


//-----------------------------------------------------------------------------
// include files for current application
#include <khgaview.h>
#include <kdevhga.h>



//-----------------------------------------------------------------------------
//
// class KHGAView
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
size_t KHGAView::WinNb=0;

//-----------------------------------------------------------------------------
KHGAView::KHGAView(KPrjView* project)
	: QMdiSubWindow(), Ui_KHGAView(), RObject("HGAView "+RString::Number(++WinNb)), Project(project), CurId(0), Instance(0), Running(false)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle("HGA Genetic Algorithm");
	Monitor->setParams(theApp->GetMaxGen(),0.0,1.0);
	Best->setProblem(project);
	Sol->setProblem(project);

	// Create GA
	try
	{
		Gen=0;
		Instance=new GInstH(theApp->GetMaxGen(),theApp->GetPopSize(),RCursor<RObjH>(Project->GetObjs()),"FirstFit",Debug);
		Instance->SetVerify(theApp->MustVerify());
		Instance->Init();
		reinterpret_cast<RObject*>(this)->InsertObserver(reinterpret_cast<tNotificationHandler>(&KHGAView::Generation),"RInst::Generation",Instance);
		reinterpret_cast<RObject*>(this)->InsertObserver(reinterpret_cast<tNotificationHandler>(&KHGAView::BestChromo),"RInst::Best",Instance);
		reinterpret_cast<RObject*>(this)->InsertObserver(reinterpret_cast<tNotificationHandler>(&KHGAView::Interact),"RInst::Interact",Instance);
	}
	catch(RException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()));
		Instance=0;
	}
	catch(exception& e)
	{
		KMessageBox::error(this,e.what());
		Instance=0;
	}
	catch(...)
	{
		KMessageBox::error(this,"Unknown Problem");
		Instance=0;
	}

	Main->setTabText(2,"Solution ("+QString::number(CurId)+"/"+QString::number(Instance->GetPopSize()-1)+")");
}


//---------------------------------------------------------------------------
void KHGAView::RunGA(void)
{
	if(Instance)
	{
		Running=true;
		try
		{
			if(theApp->GetMaxGen()>Gen)
			{
				if(theApp->GetStepGen()==0)
					Gen=theApp->GetMaxGen();
				else
				{
					Gen+=theApp->GetStepGen();
					if(Gen>theApp->GetMaxGen())
						Gen=theApp->GetMaxGen();
				}
			}
			Instance->MaxGen=Gen;
			Instance->Run();
			KMessageBox::information(this,"Done");
		}
		catch(RException& e)
		{
			KMessageBox::error(this,ToQString(e.GetMsg()));
		}
		catch(exception& e)
		{
			KMessageBox::error(this,e.what());
		}
		catch(...)
		{
			KMessageBox::error(this,"Unknown Problem");
		}
		Running=false;
	}
}


//---------------------------------------------------------------------------
void KHGAView::PauseGA(void)
{
	if(Running)
	{
		ExternBreak=true;
		Running=false;
	}
}


//---------------------------------------------------------------------------
bool KHGAView::End(void) const
{
	return(Gen==theApp->GetMaxGen());
}



//---------------------------------------------------------------------------
void KHGAView::Generation(const R::RNotification& notification)
{
	Monitor->setGenInfo(GetData<size_t>(notification),Instance->GetAgeBest(),Instance->GetBestChromosome()->Fitness->Value);
	Sol->setNodes(Instance->Chromosomes[CurId]);
}


//---------------------------------------------------------------------------
void KHGAView::BestChromo(const R::RNotification&)
{
	Best->setNodes(Instance->GetBestChromosome());
}


//---------------------------------------------------------------------------
void KHGAView::Interact(const R::RNotification&)
{
	 QCoreApplication::processEvents();
}


//---------------------------------------------------------------------------
void KHGAView::keyReleaseEvent(QKeyEvent* e)
{
	if(Main->currentWidget()!=SolTab)
	{
		QWidget::keyReleaseEvent(e);
		return;
	}
	switch(e->key())
	{
		case Qt::Key_PageUp:
			if(CurId<Instance->GetPopSize()-1) CurId++; else CurId=0;
			Main->setTabText(2,"Solution ("+QString::number(CurId)+"/"+QString::number(Instance->GetPopSize()-1)+")");
			Sol->setNodes(Instance->Chromosomes[CurId]);
			break;

		case Qt::Key_PageDown:
			if(CurId>0) CurId--; else CurId=Instance->GetPopSize()-1;
			Main->setTabText(2,"Solution ("+QString::number(CurId)+"/"+QString::number(Instance->GetPopSize()-1)+")");
			Sol->setNodes(Instance->Chromosomes[CurId]);
			break;

		default:
			e->ignore();
	}
}


//-----------------------------------------------------------------------------
KHGAView::~KHGAView()
{
	if(Instance)
		delete Instance;
}
