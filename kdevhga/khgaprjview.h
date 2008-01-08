/*

	KHGAPrjView.h

	Window showing the information on one instance - Header.

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
#ifndef KHGAPrjView_H
#define KHGAPrjView_H


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qlistview.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdevhgaview.h"


//-----------------------------------------------------------------------------
/**
* The KHGAPrjView provides a representation of a project window.
* @author Pascal Francq
*/
class KHGAPrjView : public KDevHGAView
{
	Q_OBJECT

	/**
	* List representing the elements of the projects.
	*/
	QListView *prj;

public:

	/**
	* Construct the project view.
	*/
	KHGAPrjView(KDevHGADoc* pDoc,QWidget* parent, const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual HGAViewType getType(void) {return(vProject);}

	/**
	* Ask for setting the title.
	*/
	virtual void setTitle(QString _title) {setCaption("Project: "+_title);}

	/**
	* Construct the project tree.
	*/
	void createPrj(void);

protected:

	/**
	* Handle the resize of this window.
	*/
	void resizeEvent(QResizeEvent *);

public:

	/**
	* Destruct the project view.
	*/
	~KHGAPrjView(void);
};


//-----------------------------------------------------------------------------
#endif
