/*

	KDevHGA.cpp

	Main application - Implementation.

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
// include files for Qt
#include <qdir.h>
#include <qprinter.h>
#include <qvbox.h>
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <qtoolbutton.h>
#include <qstring.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstdaction.h>
#include <kshortcut.h>
#include <kstatusbar.h>
#include <kpopupmenu.h>
#include <knuminput.h>


//-----------------------------------------------------------------------------
// Application specific includes
#include "kdevhga.h"
#include "kdevhgaview.h"
#include "khgagaview.h"
#include "khgaprjview.h"
#include "khgaheuristicview.h"
#include "kdevhgadoc.h"
#include "kappoptions.h"



//-----------------------------------------------------------------------------
//
// class KDevHGApp
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KDevHGAApp::KDevHGAApp(void)
	: KMainWindow(0,"KDevHGA")
{
	config=kapp->config();
	printer = new QPrinter;
	untitledCount=0;
	pDocList = new QList<KDevHGADoc>();
	pDocList->setAutoDelete(true);

	initStatusBar();
	initView();
	initActions();
	readOptions();

	// disable actions at startup
	heuristicFF->setEnabled(false);
	heuristicRun->setEnabled(false);
	heuristicNext->setEnabled(false);
	GAInit->setEnabled(false);
	GAStart->setEnabled(false);
	GAPause->setEnabled(false);
	GAStop->setEnabled(false);
}


//-----------------------------------------------------------------------------
void KDevHGAApp::initActions(void)
{
	// Menu "File"
	fileOpen = KStdAction::open(this, SLOT(slotFileOpen()), actionCollection());
	fileOpenRecent = KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)), actionCollection());
	fileClose = KStdAction::close(this, SLOT(slotFileClose()), actionCollection());
	fileQuit = KStdAction::quit(this, SLOT(slotFileQuit()), actionCollection());
	fileOpen->setStatusText(i18n("Opens an existing document"));
	fileOpenRecent->setStatusText(i18n("Opens a recently used file"));
	fileClose->setStatusText(i18n("Closes the actual document"));
	fileQuit->setStatusText(i18n("Quits the application"));

	// Menu "Heuristic"
	heuristicFF=new KAction(i18n("&First Fit Heuristic"),KKey("Alt+F").keyCodeQt(),this,SLOT(slotHeuristicFF(void)),actionCollection(),"heuristic_ff");
	heuristicRun=new KAction(i18n("&Run Heuristic"),"run",KKey("Alt+R").keyCodeQt(),this,SLOT(slotHeuristicRun(void)),actionCollection(),"heuristic_run");
	heuristicNext=new KAction(i18n("&Next step for Heuristic"),"next",KKey("Alt+N").keyCodeQt(),this,SLOT(slotHeuristicNext(void)),actionCollection(),"heuristic_next");

	// Menu "GA"
	GAInit=new KAction(i18n("&Initialize"),"reload",KKey("Alt+I").keyCodeQt(),this,SLOT(slotGAInit(void)),actionCollection(),"ga_init");
	GAStart=new KAction(i18n("&Start"),"exec",KKey("Alt+S").keyCodeQt(),this,SLOT(slotGAStart(void)),actionCollection(),"ga_start");
	GAPause=new KAction(i18n("&Pause"),"player_pause",KKey("Alt+P").keyCodeQt(),this,SLOT(slotGAPause(void)),actionCollection(),"ga_pause");
	GAStop=new KAction(i18n("&Stop"),"stop",KKey("Alt+T").keyCodeQt(),this,SLOT(slotGAStop(void)),actionCollection(),"ga_stop");

	// Menu "View"
	setStandardToolBarMenuEnabled(true);
	viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());
	viewStatusBar->setStatusText(i18n("Enables/disables the statusbar"));
	settingsOptions = new KAction(i18n("&Options"),"configure",0,this,SLOT(slotSettingsOptions(void)),actionCollection(),"settings_options");
	settingsOptions->setStatusText(i18n("Set the options"));

	// Menu" Window"
	windowNewWindow = new KAction(i18n("New &Window"), 0, this, SLOT(slotWindowNewWindow()), actionCollection(),"window_new_window");
	windowTile = new KAction(i18n("&Tile"), 0, this, SLOT(slotWindowTile()), actionCollection(),"window_tile");
	windowCascade = new KAction(i18n("&Cascade"), 0, this, SLOT(slotWindowCascade()), actionCollection(),"window_cascade");
	windowMenu = new KActionMenu(i18n("&Window"), actionCollection(), "window_menu");
	connect(windowMenu->popupMenu(), SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));

	createGUI();
}


//-----------------------------------------------------------------------------
void KDevHGAApp::initStatusBar(void)
{
	statusBar()->insertItem(i18n("Ready."),1);
}


//-----------------------------------------------------------------------------
void KDevHGAApp::initView(void)
{
	QVBox* view_back = new QVBox(this);
	view_back->setFrameStyle(QFrame::StyledPanel|QFrame::Sunken);
	pWorkspace = new QWorkspace(view_back);
	connect(pWorkspace,SIGNAL(windowActivated(QWidget*)),this,SLOT(slotWindowActivated(QWidget*)));
	setCentralWidget(view_back);
}


//-----------------------------------------------------------------------------
void KDevHGAApp::createClient(KDevHGADoc* doc)
{
	KHGAPrjView* w = new KHGAPrjView(doc,pWorkspace,"Project: "+doc->URL().fileName(),WDestructiveClose);
	w->installEventFilter(this);
	doc->addView(w);
	w->setIcon(kapp->miniIcon());
	w->show();
	w->resize(pWorkspace->sizeHint());
}


//-----------------------------------------------------------------------------
void KDevHGAApp::openDocumentFile(const KURL& url)
{
	KDevHGADoc* doc;

	slotStatusMsg(i18n("Opening file..."));

	// check, if document already open. If yes, set the focus to the first view
	for(doc=pDocList->first(); doc > 0; doc=pDocList->next())
	{
		if(doc->URL()==url)
		{
			KDevHGAView* view=doc->firstView();
			view->setFocus();
			return;
		}
	}
	doc = new KDevHGADoc();
	pDocList->append(doc);
	doc->newDocument();

	if(url.isEmpty())
	{
		// Creates an untitled window if file is 0	
		untitledCount+=1;
		QString fileName=QString(i18n("Untitled%1")).arg(untitledCount);
		KURL url;
		url.setFileName(fileName);
		doc->setURL(url);
	}
	else
	{
		// Open the file
		if(!doc->openDocument(url))
		{
			KMessageBox::error (this,i18n("Could not open document !"), i18n("Error !"));
			delete doc;
			return;
		}
		fileOpenRecent->addURL(url);
	}
	// create the window
	KApplication::kApplication()->processEvents();
	createClient(doc);

	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::saveOptions(void)
{	
	config->setGroup("General Options");
	config->writeEntry("Geometry", size());
	config->writeEntry("Show Toolbar", toolBar()->isVisible());
	config->writeEntry("Show Statusbar",statusBar()->isVisible());
	config->writeEntry("ToolBarPos", (int) toolBar("mainToolBar")->barPos());
	fileOpenRecent->saveEntries(config,"Recent Files");
	config->setGroup("Heuristic Options");
	config->writeEntry("Step Mode",step);
	config->setGroup("GA Options");
	config->writeEntry("Heuristic Type",GAHeur);
	config->writeEntry("Maximum Generation",GAMaxGen);
	config->writeEntry("Step Generation",GAStepGen);
	config->writeEntry("Population Size",GAPopSize);
	config->writeEntry("Verify GA",VerifyGA);
	config->writeEntry("Display Full Info",DisplayFull);
	config->writeEntry("Display Terminal Nodes",DisplayTerminals);
	config->writeEntry("Display Objects",DisplayObjects);
}


//-----------------------------------------------------------------------------
void KDevHGAApp::readOptions(void)
{

	config->setGroup("General Options");

	// bar status settings
	bool bViewToolbar = config->readBoolEntry("Show Toolbar", true);
	if(bViewToolbar)
		toolBar()->show();
	else
		toolBar()->hide();
	bool bViewStatusbar = config->readBoolEntry("Show Statusbar", true);
	viewStatusBar->setChecked(bViewStatusbar);
	slotViewStatusBar();

	// bar position settings
	KToolBar::BarPosition toolBarPos;
	toolBarPos=(KToolBar::BarPosition) config->readNumEntry("ToolBarPos", KToolBar::Top);
	toolBar("mainToolBar")->setBarPos(toolBarPos);

	// initialize the recent file list
	fileOpenRecent->loadEntries(config,"Recent Files");

	QSize size=config->readSizeEntry("Geometry");
	if(!size.isEmpty())
	{
		resize(size);
	}

	// Specific settings
	config->setGroup("Heuristic Options");
	step=config->readBoolEntry("Step Mode",false);
	config->setGroup("GA Options");
	GAHeur=static_cast<HeuristicType>(config->readNumEntry("Heuristic Type",FirstFit));
	GAMaxGen=config->readUnsignedLongNumEntry("Maximum Generation",100);
	GAStepGen=config->readUnsignedLongNumEntry("Step Generation",0);
	GAPopSize=config->readUnsignedLongNumEntry("Population Size",16);
	VerifyGA=config->readBoolEntry("Verify GA",false);
	DisplayFull=config->readBoolEntry("Display Full Info",true);
	DisplayTerminals=config->readBoolEntry("Display Terminal Nodes",true);
	DisplayObjects=config->readBoolEntry("Display Objects",true);
}


//-----------------------------------------------------------------------------
void KDevHGAApp::saveProperties(KConfig* /*_cfg*/)
{
}


//-----------------------------------------------------------------------------
void KDevHGAApp::readProperties(KConfig* /*_cfg*/)
{
}


//-----------------------------------------------------------------------------
bool KDevHGAApp::queryClose(void)
{
	QStringList saveFiles;
	KDevHGADoc* doc;

	if(pDocList->isEmpty())
	return true;

	for(doc=pDocList->first(); doc!=0;doc=pDocList->next())
	{
		if(doc->isModified())
			saveFiles.append(doc->URL().fileName());
	}
	if(saveFiles.isEmpty())
		return true;
}


//-----------------------------------------------------------------------------
bool KDevHGAApp::queryExit(void)
{
	saveOptions();
	return true;
}


//-----------------------------------------------------------------------------
bool KDevHGAApp::eventFilter(QObject* object, QEvent* event)
{
	if((event->type() == QEvent::Close)&&((KDevHGAApp*)object!=this))
	{
		QCloseEvent* e=(QCloseEvent*)event;

		KDevHGAView* pView=(KDevHGAView*)object;
		KDevHGADoc* pDoc=pView->getDocument();
		if(pDoc->canCloseFrame(pView))
		{
			pDoc->removeView(pView);
			if(!pDoc->firstView())
				pDocList->remove(pDoc);
			e->accept();
		}
		else
			e->ignore();
	}
	return(QWidget::eventFilter(object,event));    // standard event processing
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotHeuristicFF(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==vProject))
	{
		KDevHGADoc* doc = m->getDocument();
		KHGAHeuristicView* w = new KHGAHeuristicView(doc,FirstFit,pWorkspace,0,WDestructiveClose);
		w->installEventFilter(this);
		doc->addView(w);
		w->setIcon(kapp->miniIcon());
		w->resize(pWorkspace->sizeHint());
		w->show();
		w->setFocus();
		//w->RunHeuristic();
	}
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotEndHeuristic(void)
{
	bool bRun=false;
	KDevHGAView* v;
	QWidgetList list;

	KApplication::kApplication()->processEvents(1000);

	// Scan all documents to see if all heuristics are end.
	list=pWorkspace->windowList();
	for(v=(KDevHGAView*)list.first();v!=0;v=(KDevHGAView*)list.next())
	{
		if(v->getType()==vHeuristic)
		{
			if(((KHGAHeuristicView*)v)->Running())
				bRun=true;
		}
	}
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotHeuristicNext(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==vHeuristic))
	{
		((KHGAHeuristicView*)m)->NextStep();
	}
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotHeuristicRun(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==vHeuristic))
	{
		((KHGAHeuristicView*)m)->RunToEnd();
	}
}

//-----------------------------------------------------------------------------
void KDevHGAApp::slotGAInit(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==vProject))
	{
		KDevHGADoc* doc = m->getDocument();
		KHGAGAView* w = new KHGAGAView(doc,pWorkspace,0,WDestructiveClose);
		w->installEventFilter(this);
		doc->addView(w);
		w->setIcon(kapp->miniIcon());
		w->resize(pWorkspace->sizeHint());
		w->show();
		w->setFocus();
	}
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotGAStart(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==vGA))
	{
		((KHGAGAView*)m)->RunGA();
	}
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotGAPause(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==vGA))
	{
		((KHGAGAView*)m)->PauseGA();
	}
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotGAStop(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==vGA))
	{
		((KHGAGAView*)m)->StopGA();
	}
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotSettingsOptions(void)
{
	slotStatusMsg(i18n("Set the options..."));
	KAppOptions dlg(this,"Options",true);
	dlg.cbStep->setChecked(step);
	dlg.txtMaxGen->setValue(GAMaxGen);
	dlg.txtStepGen->setValue(GAStepGen);
	dlg.cbGAHeuristicType->setCurrentItem(GAHeur);
	dlg.txtPopSize->setValue(GAPopSize);
	dlg.VerifyGA->setChecked(VerifyGA);
	dlg.DisplayFull->setChecked(DisplayFull);
	dlg.DisplayTerminals->setChecked(DisplayTerminals);
	dlg.DisplayObjects->setChecked(DisplayObjects);
	bool HoldFull=DisplayFull;
	bool HoldTerminals=DisplayTerminals;
	bool HoldObjects=DisplayObjects;
	if(dlg.exec())
	{
		step=dlg.cbStep->isChecked();
		GAMaxGen=dlg.txtMaxGen->value();
		GAStepGen=dlg.txtStepGen->value();
		GAHeur=static_cast<HeuristicType>(dlg.cbGAHeuristicType->currentItem());
		GAPopSize=dlg.txtPopSize->value();
		VerifyGA=dlg.VerifyGA->isChecked();
		DisplayFull=dlg.DisplayFull->isChecked();
		DisplayTerminals=dlg.DisplayTerminals->isChecked();
		DisplayObjects=dlg.DisplayObjects->isChecked();
		if((HoldFull!=DisplayFull)||(HoldTerminals!=DisplayTerminals)||(HoldObjects!=DisplayObjects))
			emit redrawTrees();	
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotFileOpen(void)
{
	slotStatusMsg(i18n("Opening file..."));
	KURL url=KFileDialog::getOpenURL("/home/pfrancq/data/data/hga/",i18n("*.hd|Hierarchical Data files"), this, i18n("Open File..."));
	if(!url.isEmpty())
	{
		KApplication::kApplication()->processEvents();
		openDocumentFile(url);
		fileOpenRecent->addURL( url );
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotFileOpenRecent(const KURL& url)
{
	slotStatusMsg(i18n("Opening file..."));
	KApplication::kApplication()->processEvents();
	openDocumentFile(url);
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotFileClose(void)
{
	slotStatusMsg(i18n("Closing file..."));
	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m)
	{
		KDevHGADoc* doc=m->getDocument();
		doc->closeDocument();
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotFileQuit(void)
{
	slotStatusMsg(i18n("Exiting..."));
	saveOptions();
	close();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotViewStatusBar(void)
{
	slotStatusMsg(i18n("Toggle the statusbar..."));
	if(!viewStatusBar->isChecked())
		statusBar()->hide();
	else
		statusBar()->show();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotWindowNewWindow(void)
{
	slotStatusMsg(i18n("Opening a new application window..."));
	KDevHGAView* m = (KDevHGAView*) pWorkspace->activeWindow();
	if(m)
	{
		KDevHGADoc* doc = m->getDocument();
		createClient(doc);
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotWindowTile(void)
{
	pWorkspace->tile();
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotWindowCascade(void)
{
	pWorkspace->cascade();
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotStatusMsg(const QString& text)
{
	statusBar()->clear();
	statusBar()->changeItem(text,1);
}


//-----------------------------------------------------------------------------
void KDevHGAApp::windowMenuAboutToShow(void)
{
	windowMenu->popupMenu()->clear();
	windowMenu->insert(windowNewWindow);
	windowMenu->insert(windowCascade);
	windowMenu->insert(windowTile);
	if(pWorkspace->windowList().isEmpty())
	{
		windowNewWindow->setEnabled(false);
		windowCascade->setEnabled(false);
		windowTile->setEnabled(false);
	}
	else
	{
		windowNewWindow->setEnabled(true);
		windowCascade->setEnabled(true);
		windowTile->setEnabled(true);
	}
	windowMenu->popupMenu()->insertSeparator();
	QWidgetList windows = pWorkspace->windowList();
	for(int i=0;i<int(windows.count());i++)
	{
		int id=windowMenu->popupMenu()->insertItem(QString("&%1 ").arg(i+1)+windows.at(i)->caption(),this,SLOT(windowMenuActivated(int)));
		windowMenu->popupMenu()->setItemParameter(id,i);
		windowMenu->popupMenu()->setItemChecked(id,pWorkspace->activeWindow()==windows.at(i));
  }
}


//-----------------------------------------------------------------------------
void KDevHGAApp::windowMenuActivated(int id)
{
	QWidget* w=pWorkspace->windowList().at(id);
	if(w)
		w->setFocus();
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotWindowActivated(QWidget*)
{
	bool bPrj,bGA,bHeuristic;

	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m)
	{
		// Update caption
		setCaption(m->caption());

		// Update menu
		switch(m->getType())
		{
			case vProject:
				bPrj=true;
				bGA=false;
				bHeuristic=false;
				break;

			case vHeuristic:
				bPrj=false;
				bGA=false;
				bHeuristic=true;
				break;

			case vGA:
				bPrj=false;
				bGA=true;
				bHeuristic=false;
				break;

			default:
				bPrj=false;
				bGA=false;
				bHeuristic=false;
				break;
		}
		GAInit->setEnabled(bPrj);
		heuristicFF->setEnabled(bPrj);
		GAStart->setEnabled(bGA);
		GAPause->setEnabled(bGA);
		GAStop->setEnabled(bGA);
		heuristicRun->setEnabled(bHeuristic);
		heuristicNext->setEnabled(bHeuristic);
	}
	else
	{
		setCaption("");
		GAInit->setEnabled(false);
		heuristicFF->setEnabled(false);
		GAStart->setEnabled(false);
		GAPause->setEnabled(false);
		GAStop->setEnabled(false);
		heuristicRun->setEnabled(false);
		heuristicNext->setEnabled(false);
	}
}


//-----------------------------------------------------------------------------
KDevHGAApp::~KDevHGAApp(void)
{
	delete printer;
	delete pDocList;
}
