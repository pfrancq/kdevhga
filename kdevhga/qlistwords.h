/*

	R Project Library

	qlistwords.h

	Description - Header.

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
#ifndef QLISTWORDS_H
#define QLISTWORDS_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rhga/rnodesga.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qlistview.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "gawords.h"


//-----------------------------------------------------------------------------
// forward class declaration
class KDevHGADoc;


//-----------------------------------------------------------------------------
/**
* @author Pascal Francq
*/
class QListWords : public QListView
{
	Q_OBJECT
	KDevHGADoc* Doc;
	RNodesGA<RNodeWords,RObjH,RNodeWordsData>* Nodes;

	void ConstNode(QListViewItem* p,QListViewItem*& cur,RNodeWords* n);

public:
	QListWords(KDevHGADoc* pDoc,QWidget* parent=0);
	void setNodes(RNodesGA<RNodeWords,RObjH,RNodeWordsData>* nodes);
	~QListWords(void);
};


//-----------------------------------------------------------------------------
#endif
