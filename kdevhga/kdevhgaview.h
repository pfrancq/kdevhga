/*

	KDevHGAView.h

	Generic Window - Header.

	Copyright 1998-2004 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef KDEVHGAVIEW_H
#define KDEVHGAVIEW_H


//------------------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//------------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>


//------------------------------------------------------------------------------
// Enum declaration
/**
* Defines all the possible type of view for a document.
*/
enum HGAViewType{Nothing,Project,Heuristic,GA};


//------------------------------------------------------------------------------
class KDevHGADoc;


//------------------------------------------------------------------------------
/**
* The KDevHGAView class provides the view widget for the document instance
* connected to it and is displayed as a MDI child window in the main view area
* of the KDevHGAApp class instance. The KDevHGAApp class also has an
* eventFilter() method that gets installed on every KDevHGAView instance to
* control events of the type QEvent::Close.
* @author Pascal Francq
* @short Generic View for HGA.
*/
class KDevHGAView : public QWidget
{
	Q_OBJECT

protected:

	/**
	* The document connected to the view, specified in the constructor.
	*/
	KDevHGADoc *doc;

public:

	/**
	* Constructor for the view.
	* @param pDoc           your document instance that the view represents.
	*                       Create a document before calling the constructor or
	*                       connect an already existing document to a new MDI
	*                       child widget.
	*/
	KDevHGAView(KDevHGADoc* pDoc,QWidget* parent,const char* name,int wflags);

	/**
	* Returns a pointer to the document connected to the view.
	*/
	KDevHGADoc* getDocument(void) const;

	/**
	* Gets called to redraw the document contents if it has been modified.
	*/
	void update(KDevHGAView* pSender);

	/**
	* Contains the implementation for printing functionality and gets called by
	* KDevHGAApp::slotFilePrint().
	*/
	void print(QPrinter* pPrinter);

	/**
	* Return the type of the window.
	*/
	virtual HGAViewType getType(void)=0;

protected:

	/**
	* Overwritten QWidget::closeEvent() to catch closing views. Does nothing,
	* as the closeEvents for KDevHGAView's are processed by
	* KDevHGAApp::eventFilter(), so this overwitten closeEvent is necessary and
	* has to be empty. Don't overwrite this method !
	*/
	virtual void closeEvent(QCloseEvent* e);

public:

	/**
	* Destructor for the main view.
	*/
	~KDevHGAView(void);

	// friend classes
	friend class KDevHGADoc;
};


//-----------------------------------------------------------------------------
#endif // KDEVHGAVIEW_H
