/*

	R Project Library

	kdevhga.cpp

	Description - Implementation.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

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


//-----------------------------------------------------------------------------
// application specific includes
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
	fileSave->setEnabled(false);
	fileSaveAs->setEnabled(false);
	filePrint->setEnabled(false);
	editCut->setEnabled(false);
	editCopy->setEnabled(false);
	editPaste->setEnabled(false);
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
	fileNew = KStdAction::openNew(this, SLOT(slotFileNew()), actionCollection());
	fileOpen = KStdAction::open(this, SLOT(slotFileOpen()), actionCollection());
	fileOpenRecent = KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)), actionCollection());
	fileSave = KStdAction::save(this, SLOT(slotFileSave()), actionCollection());
	fileSaveAs = KStdAction::saveAs(this, SLOT(slotFileSaveAs()), actionCollection());
	fileClose = KStdAction::close(this, SLOT(slotFileClose()), actionCollection());
	filePrint = KStdAction::print(this, SLOT(slotFilePrint()), actionCollection());
	fileQuit = KStdAction::quit(this, SLOT(slotFileQuit()), actionCollection());
	fileNew->setStatusText(i18n("Creates a new document"));
	fileOpen->setStatusText(i18n("Opens an existing document"));
	fileOpenRecent->setStatusText(i18n("Opens a recently used file"));
	fileSave->setStatusText(i18n("Saves the actual document"));
	fileSaveAs->setStatusText(i18n("Saves the actual document as..."));
	fileClose->setStatusText(i18n("Closes the actual document"));
	filePrint ->setStatusText(i18n("Prints out the actual document"));
	fileQuit->setStatusText(i18n("Quits the application"));

	// Menu "Edit"
	editCut = KStdAction::cut(this, SLOT(slotEditCut()), actionCollection());
	editCopy = KStdAction::copy(this, SLOT(slotEditCopy()), actionCollection());
	editPaste = KStdAction::paste(this, SLOT(slotEditPaste()), actionCollection());
	editCut->setStatusText(i18n("Cuts the selected section and puts it to the clipboard"));
	editCopy->setStatusText(i18n("Copies the selected section to the clipboard"));
	editPaste->setStatusText(i18n("Pastes the clipboard contents to actual position"));

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
	viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar()), actionCollection());
	viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());
	settingsOptions = new KAction(i18n("&Options"),"configure",0,this,SLOT(slotSettingsOptions(void)),actionCollection(),"settings_options");
	viewToolBar->setStatusText(i18n("Enables/disables the toolbar"));
	viewStatusBar->setStatusText(i18n("Enables/disables the statusbar"));
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
}


//-----------------------------------------------------------------------------
void KDevHGAApp::readOptions(void)
{

	config->setGroup("General Options");

	// bar status settings
	bool bViewToolbar = config->readBoolEntry("Show Toolbar", true);
	viewToolBar->setChecked(bViewToolbar);
	slotViewToolBar();

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

	switch(KMessageBox::questionYesNoList(this,i18n("One or more documents have been modified.\nSave changes before exiting?"),saveFiles))
	{
		case KMessageBox::Yes:
			for(doc=pDocList->first(); doc!=0;doc=pDocList->next())
			{
				if(doc->URL().fileName().contains(i18n("Untitled")))
					slotFileSaveAs();
				else
				{
					if(!doc->saveDocument(doc->URL()))
					{
						KMessageBox::error (this,i18n("Could not save the current document !"), i18n("I/O Error !"));
						return false;
					}
				 }
			}
			return true;

		case KMessageBox::No:
		default:
			return true;
	}
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
	if(m&&(m->getType()==Project))
	{
		KDevHGADoc* doc = m->getDocument();
		KHGAHeuristicView* w = new KHGAHeuristicView(doc,FirstFit,pWorkspace,0,WDestructiveClose);
		w->installEventFilter(this);
		doc->addView(w);
		w->setIcon(kapp->miniIcon());
		w->resize(pWorkspace->sizeHint());
		w->show();
		w->setFocus();
		w->RunHeuristic();
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
		if(v->getType()==Heuristic)
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
	if(m&&(m->getType()==Heuristic))
	{
		((KHGAHeuristicView*)m)->NextStep();
	}
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotHeuristicRun(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==Heuristic))
	{
		((KHGAHeuristicView*)m)->RunToEnd();
	}
}

//-----------------------------------------------------------------------------
void KDevHGAApp::slotGAInit(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==Project))
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
	if(m&&(m->getType()==GA))
	{
		((KHGAGAView*)m)->RunGA();
	}
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotGAPause(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==GA))
	{
		((KHGAGAView*)m)->PauseGA();
	}
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotGAStop(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==GA))
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
	dlg.txtMaxGen->setText(QString::number(GAMaxGen));
	dlg.txtStepGen->setText(QString::number(GAStepGen));
	dlg.cbGAHeuristicType->setCurrentItem(GAHeur);
	dlg.txtPopSize->setText(QString::number(GAPopSize));
	if(dlg.exec())
	{
		step=dlg.cbStep->isChecked();
		GAMaxGen=dlg.txtMaxGen->text().toULong();
		GAStepGen=dlg.txtStepGen->text().toULong();
		GAHeur=static_cast<HeuristicType>(dlg.cbGAHeuristicType->currentItem());
		GAPopSize=dlg.txtPopSize->text().toULong();
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotFileNew(void)
{
	slotStatusMsg(i18n("Creating new document..."));
	openDocumentFile();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotFileOpen(void)
{
	slotStatusMsg(i18n("Opening file..."));
	KURL url=KFileDialog::getOpenURL("/home/pfrancq/data/projects/hp/data",i18n("*.hd|Hierarchical Data files"), this, i18n("Open File..."));
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
void KDevHGAApp::slotFileSave(void)
{
	slotStatusMsg(i18n("Saving file..."));
	KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
	if(m)
	{
		KDevHGADoc* doc = m->getDocument();
		if(doc->URL().fileName().contains(i18n("Untitled")))
			slotFileSaveAs();
		else
			if(!doc->saveDocument(doc->URL()))
				KMessageBox::error (this,i18n("Could not save the current document !"), i18n("I/O Error !"));
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotFileSaveAs(void)
{
	slotStatusMsg(i18n("Saving file with a new filename..."));
	KURL url=KFileDialog::getSaveURL(QDir::currentDirPath(),i18n("*|All files"), this, i18n("Save as..."));
	if(!url.isEmpty())
	{
		KDevHGAView* m = (KDevHGAView*)pWorkspace->activeWindow();
		if(m)
		{
			KDevHGADoc* doc =	m->getDocument();
			if(!doc->saveDocument(url))
			{
				KMessageBox::error (this,i18n("Could not save the current document !"), i18n("I/O Error !"));
				return;
			}
			doc->changedViewList();
//			setWndTitle(m);
			fileOpenRecent->addURL(url);
		}
	}
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
void KDevHGAApp::slotFilePrint(void)
{
	slotStatusMsg(i18n("Printing..."));
	KDevHGAView* m = (KDevHGAView*) pWorkspace->activeWindow();
	if(m)
		m->print( printer );
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotFileQuit(void)
{
	slotStatusMsg(i18n("Exiting..."));
	saveOptions();
	// close the first window, the list makes the next one the first again.
	// This ensures that queryClose() is called on each window to ask for closing
	KMainWindow* w;
	if(memberList)
	{
		for(w=memberList->first(); w!=0; w=memberList->first())
		{
			// only close the window if the closeEvent is accepted. If the user presses Cancel on the saveModified() dialog,
			// the window and the application stay open.
			if(!w->close())
				break;
		}
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotEditUndo(void)
{
	slotStatusMsg(i18n("Reverting last action..."));
	KDevHGAView* m = (KDevHGAView*) pWorkspace->activeWindow();
	if(m)
//		m->undo();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotEditCut(void)
{
	slotStatusMsg(i18n("Cutting selection..."));
	KDevHGAView* m = (KDevHGAView*) pWorkspace->activeWindow();
	if(m)
//		m->cut();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotEditCopy(void)
{
	slotStatusMsg(i18n("Copying selection to clipboard..."));
	KDevHGAView* m = (KDevHGAView*) pWorkspace->activeWindow();
	if(m)
//		m->copy();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotEditPaste(void)
{
  slotStatusMsg(i18n("Inserting clipboard contents..."));
	
  KDevHGAView* m = (KDevHGAView*) pWorkspace->activeWindow();
  if ( m )
//    m->paste();
		
  slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevHGAApp::slotViewToolBar(void)
{
	slotStatusMsg(i18n("Toggle the toolbar..."));
	if(!viewToolBar->isChecked())
		toolBar("mainToolBar")->hide();
	else
		toolBar("mainToolBar")->show();
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
			case Project:
				bPrj=true;
				bGA=false;
				bHeuristic=false;
				break;

			case Heuristic:
				bPrj=false;
				bGA=false;
				bHeuristic=true;
				break;

			case GA:
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
}
