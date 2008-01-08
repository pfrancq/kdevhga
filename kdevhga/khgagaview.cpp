/*

	KHGAGAView.cpp

	Windows showig the running HGA - Implementation.

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
// include files for GALILEI
#include <ginsth.h>
#include <gchromoh.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
#include <klocale.h>
#include <kmessagebox.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdevhga.h"
#include "khgagaview.h"
#include "kdevhgadoc.h"



//-----------------------------------------------------------------------------
//
// class KHGAGAView
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KHGAGAView::KHGAGAView(KDevHGADoc* pDoc,QWidget *parent, const char *name,int wflags)
	: KDevHGAView(pDoc,parent,name,wflags), RObject(name), CurId(0), Instance(0)
{
	static char tmp[100];

	TabWidget = new QTabWidget( this, "TabWidget" );
	TabWidget->setGeometry(rect());
	TabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, TabWidget->sizePolicy().hasHeightForWidth() ) );
	TabWidget->setBackgroundOrigin( QTabWidget::ParentOrigin );

	// Stat part
	StatSplitter=new QSplitter(QSplitter::Vertical,TabWidget,"Statistic");
	TabWidget->insertTab(StatSplitter,"Statistic");
	StatSplitter->setGeometry(rect());
	Monitor=new	QGAMonitor(StatSplitter);
	Monitor->setMaxGen(theApp->GAMaxGen);
	connect(this,SIGNAL(signalSetGen(const unsigned int,const unsigned int,const double)),Monitor,SLOT(slotSetGen(const unsigned int,const unsigned int,const double)));
	Debug=new QXMLContainer(StatSplitter);

	// Solution part
	Best = new QTreeInfos(pDoc,TabWidget);
	TabWidget->insertTab(Best,"Best Solution");

	// Solution part
	Sol = new QTreeInfos(pDoc,TabWidget);
	sprintf(tmp,"Solution (0/%u)",((KDevHGAApp*)parentWidget()->parentWidget()->parentWidget())->GAPopSize-1);
	TabWidget->insertTab(Sol,tmp);

	// Create GA
	try
	{
		Gen=0;
		Instance=new GInstH(theApp->GAMaxGen,theApp->GAPopSize,RCursor<RObjH>(*pDoc->Objs),FirstFit,Debug);
		Instance->SetVerify(theApp->VerifyGA);
		Monitor->setMaxFitness(Instance->GetMaxAttr()*2);
		reinterpret_cast<RObject*>(this)->InsertObserver(HANDLER(KHGAGAView::GenSig),"RInst::Generation",Instance);
		reinterpret_cast<RObject*>(this)->InsertObserver(HANDLER(KHGAGAView::InteractSig),"RInst::Interact",Instance);
		reinterpret_cast<RObject*>(this)->InsertObserver(HANDLER(KHGAGAView::BestSig),"RInst::Best",Instance);	
		Instance->Init();
	}
	catch(RHGAException& e)
	{
		KMessageBox::error(this,e.GetMsg());
		Instance=0;
	}
	catch(std::bad_alloc)
	{
		KMessageBox::error(this,"Memory Problems");
		Instance=0;
	}
	catch(...)
	{
		KMessageBox::error(this,"Unknown Problem");
		Instance=0;
	}
}


//---------------------------------------------------------------------------
void KHGAGAView::GenSig(const RNotification&)
{
	emit signalSetGen(Instance->GetGen(),Instance->GetAgeBest(),Instance->BestChromosome->Fitness->Value);
	Sol->setNodes(Instance->Chromosomes[CurId]);
}


//---------------------------------------------------------------------------
void KHGAGAView::InteractSig(const RNotification&)
{
	KApplication::kApplication()->processEvents(1000);
}


//---------------------------------------------------------------------------
void KHGAGAView::BestSig(const RNotification&)
{
	static char tmp[100];
	sprintf(tmp,"Best Solution (Id=%u)",Instance->BestChromosome->Id);
	TabWidget->changeTab(Best,tmp);
	Best->setNodes(Instance->BestChromosome);
}


//---------------------------------------------------------------------------
void KHGAGAView::RunGA(void)
{
	if(Instance)
	{
		try
		{
			if(theApp->GAMaxGen>Gen)
			{
				if(theApp->GAStepGen==0)
					Gen=theApp->GAMaxGen;
				else
				{
					Gen+=theApp->GAStepGen;
					if(Gen>theApp->GAMaxGen) Gen=theApp->GAMaxGen;
				}
			}
			Instance->MaxGen=Gen;
			Instance->Run();
			if(Gen==theApp->GAMaxGen)
				theApp->GAPause->setEnabled(false);
			Best->setNodes(Instance->BestChromosome);
			Sol->setNodes(Instance->Chromosomes[CurId]);
			KMessageBox::information(this,"Done");
		}
		catch(RGAException& e)
		{
			KMessageBox::error(this,e.GetMsg());
		}
	}
}


//---------------------------------------------------------------------------
void KHGAGAView::PauseGA(void)
{
	ExternBreak=true;
}


//---------------------------------------------------------------------------
void KHGAGAView::StopGA(void)
{
}


//---------------------------------------------------------------------------
void KHGAGAView::keyReleaseEvent(QKeyEvent* e)
{
	static char tmp[100];
//	QGoToPopDlg *dlg;

	if(TabWidget->currentPage()!=Sol)
	{
		KDevHGAView::keyReleaseEvent(e);
		return;
	}
	switch(e->key())
	{
		case Key_PageUp:
			if(CurId<Instance->GetPopSize()-1) CurId++; else CurId=0;
			sprintf(tmp,"Solution (%u/%u)",CurId,Instance->GetPopSize()-1);
			TabWidget->changeTab(Sol,tmp);
			Sol->setNodes(Instance->Chromosomes[CurId]);
			break;

		case Key_PageDown:
			if(CurId>0) CurId--; else CurId=Instance->GetPopSize()-1;
			sprintf(tmp,"Solution (%u/%u)",CurId,Instance->GetPopSize()-1);
			TabWidget->changeTab(Sol,tmp);
			Sol->setNodes(Instance->Chromosomes[CurId]);
			break;

//		case Key_G:
//			if(e->state()==ControlButton)
//			{
//				dlg= new QGoToPopDlg(Doc->getPopSize());
//				if(dlg->exec())
//				{
//					CurId=dlg->PopIndex->value();
//					slotNewChromo();
//				}
//				delete dlg;
//			}
//			break;

		default:
			e->ignore();
	}
}


//-----------------------------------------------------------------------------
void KHGAGAView::resizeEvent(QResizeEvent*)
{
	TabWidget->resize(size());
}


//-----------------------------------------------------------------------------
KHGAGAView::~KHGAGAView(void)
{
	if(Instance)
		delete Instance;
}
