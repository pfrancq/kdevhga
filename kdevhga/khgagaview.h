/*

	KHGAGAView.h

	Windows showig the running HGA - Header.

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
#ifndef KHGAGAView_H
#define KHGAGAView_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <robject.h>
using namespace R;


//-----------------------------------------------------------------------------
#include <ghga.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qtabwidget.h>


//-----------------------------------------------------------------------------
// include files for Qt Widgets
#include <qgamonitor.h>
#include <qxmlcontainer.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdevhgaview.h"
#include <qtreeinfos.h>

	
//-----------------------------------------------------------------------------
/**
* The KHGAGAView class provides a representation of a result of a GA that is
* running.
* @author Pascal Francq
*/
class KHGAGAView : public KDevHGAView, public RObject
{
	Q_OBJECT

	/**
	* The tab widget.
	*/
	QTabWidget* TabWidget;
	
	/**
	* The widget that handle statistics about the running GA.	
	*/
	QGAMonitor* Monitor;
	
	/**
	* The widget that handle the debug information of the running GA.
	*/
	QXMLContainer* Debug;
	
	/**
	* The splitter that separate the monitor and the debug part.
	*/
	QSplitter* StatSplitter;
	
	/**
	* The best solution.
	*/
	QTreeInfos* Best;
	
	/**
	* The solutions.
	*/
	QTreeInfos* Sol;
	
	/**
	* Identificator of the current showed solution.
	*/
	unsigned int CurId;

	/**
	* The GA that will be used.
	*/
	GInstH* Instance;

	/**
	* Number of generation already executed.
	*/
	unsigned int Gen;
	
public:

	/**
	* Constructor for the view.
	* @param pDoc           your document instance that the view represents. Create a
	*                       document before calling the constructor or connect an
	*                       already existing document to a new MDI child widget.
	*/
	KHGAGAView(KDevHGADoc* pDoc,QWidget* parent, const char* name,int wflags);

	virtual RCString GetClassName(void) const {return("KHGAGAView");}
	
	/**
	* Return the type of the window.
	*/
	virtual HGAViewType getType(void) {return(vGA);}

	/**
	* Run the GA.
	*/
	void RunGA(void);

	/**
	* Pause the GA.
	*/
	void PauseGA(void);

	/**
	* Stop the GA.
	*/
	void StopGA(void);

protected:

	/**
	* Key release event method. The implementation is needed to change the
	* chromosome to show when the user uses the keys "Page Up" and "Page Down".
	* With "Ctrl-G", a dialog box appears to select directly the identificator.
	*/
	virtual void keyReleaseEvent(QKeyEvent* e);

	/**
	* The function that handle the resize event.
	*/
	virtual void resizeEvent(QResizeEvent*);
	
	void GenSig(const RNotification& notification);
	void InteractSig(const RNotification& notification);
	void BestSig(const RNotification& notification);	
	
signals:

	/**
	* Signal to emit after a generation is done.
	*/
	void signalSetGen(const unsigned int gen,const unsigned int best,const double value);

public:

	/**
	* Destruct the view.
	*/
	~KHGAGAView(void);

	// friend classes
	friend class KDevHGADoc;
};


//-----------------------------------------------------------------------------
#endif
