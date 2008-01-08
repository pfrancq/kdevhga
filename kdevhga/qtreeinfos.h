/*

	QTreeInfos.h

	Widget representing a tree of vectors - Header.

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
#ifndef QTreeInfos_H
#define QTreeInfos_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rnodesga.h>
#include <robjh.h>
#include <rattrlist.h>
using namespace R;

//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qlistview.h>


//-----------------------------------------------------------------------------
// include files for current application
#include <gnodeinfos.h>
#include <ghga.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// forward class declaration
class KDevHGADoc;
class KHGAHeuristicView;


//-----------------------------------------------------------------------------
/**
* @author Pascal Francq
*/
class QTreeInfos : public QListView
{
	Q_OBJECT
	KDevHGADoc* Doc;
	GChromoH* Chromos;

	void constObjs(RCursor<RObjH> objs,QListViewItem* item);
	void constNode(QListViewItem* p,QListViewItem*& cur,GNodeInfos* n);

public:
	
	QTreeInfos(KDevHGADoc* pDoc,QWidget* parent=0);
	void setNodes(GChromoH* chromos);
	
protected slots:

	/**
	* Mouse button press event method.
	* @param e              MouseEvent info.
	*/
	void slotPressEvent(QListViewItem* item,const QPoint& pt,int col);

	void redraw(void);
	
public:
	
	~QTreeInfos(void);
};


//-----------------------------------------------------------------------------
#endif
