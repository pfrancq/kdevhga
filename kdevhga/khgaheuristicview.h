/*

	KHGAHeuristicView.h

	Window to follow the steps of an heuristic - Header.

	Copyright 1998-2004 by the Universit�Libre de Bruxelles.

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
#ifndef KHGAHEURISTICVIEW_H
#define KHGAHEURISTICVIEW_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/random.h>
#include <rhga/rhga.h>
#include <rhga/rtreeheuristic.h>
#include <gnodeinfos.h>
#include <rhga/rnodesga.h>
#include <rhga/rnodega.h>
#include <ghga.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qlabel.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdevhgaview.h"
#include <qtreeinfos.h>


//-----------------------------------------------------------------------------
class KHGAHeuristicView;


//-----------------------------------------------------------------------------
class MyNode : public RNodeGA<MyNode,RObjH,GNodeInfosData,KHGAHeuristicView>
{
public:
	/**
	* Construct the node.
	* @param owner          Owner of the node.
	* @param id             Identificator of the node.
	* @param data           Data used to construct the node.
	*/
	MyNode(RNodesGA<MyNode,RObjH,GNodeInfosData,KHGAHeuristicView>* owner,unsigned int id,GNodeInfosData* data);

	MyNode(const MyNode* w);

	int Compare(const MyNode* n);
};


//-----------------------------------------------------------------------------
/**
* The KVLSIHeuristicView class provides a representation of a result of a
* specific heuristic.
* @author Pascal Francq
*/
class KHGAHeuristicView : public KDevHGAView, public RNodesGA<MyNode,RObjH,GNodeInfosData,KHGAHeuristicView>
{
	Q_OBJECT

	/**
	* Random number generator.
	*/
	RRandom* Random;

	/**
	* Type of the heuristic.
	*/
	HeuristicType type;

	/**
	* Number of objects.
	*/
	unsigned int nbObjs;

	/**
	* Show information about the result.
	*/
	QLabel* result;

	/**
	* Show the polygons.
	*/
	QTreeInfos* draw;

	/**
	* Step Mode.
	*/
	bool step;

	/**
	* Stop the heuristic.
	*/
	bool Stop;

	/**
	* Data needed for the construction of the groups.
	*/
	GNodeInfosData* Data;

	/**
	* Heuristic used.
	*/
	RTreeHeuristic<MyNode,RObjH,GNodeInfosData,KHGAHeuristicView>* TreeHeur;

	/**
	* Objects to group.
	*/
	RCursor<RObjH,unsigned int>* Objs;

public:

	/**
	* Construct a heuristic view.
	*/
	KHGAHeuristicView(KDevHGADoc* pDoc,HeuristicType pType,QWidget* parent, const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual HGAViewType getType(void) {return(Heuristic);}

	/**
	* Ask for setting the title.
	*/
	virtual void setTitle(QString _title);

	/**
	* Start the heuristic.
	*/
	void RunHeuristic(void);

	/**
	* Next step for the choosen Heuristic.
	*/
	void NextStep(void);

	/**
	* Run the heuristic to the end.
	*/
	void RunToEnd(void);

	/**
	* See if the heurisitic is running or not.
	* @return true if the heuristic is running.
	*/
	bool Running(void) {return(!TreeHeur->IsEnd());}

protected:

	/**
	* Resize event method.
	*/
	virtual void resizeEvent(QResizeEvent *);

signals:

	/**
	* This signal indicates that the heuristic has finish to run.
	*/
	void endRun(void);

public:

	/**
	*  Destruct a heuristic view.
	*/
	~KHGAHeuristicView();
};


//-----------------------------------------------------------------------------
#endif
