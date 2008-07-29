/*

	KHGAHeuristicView.h

	Window to follow the steps of an heuristic - Header.

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
#ifndef KHGAHeuristicView_H
#define KHGAHeuristicView_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <random.h>
#include <rhga.h>
#include <rtreeheuristic.h>
#include <gnodeinfos.h>
#include <rnodesga.h>
#include <rnodega.h>
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
#include "gchromoh.h"
#include "ginsth.h"
#include "gnodeinfos.h"
#include <qtreeinfos.h>


//-----------------------------------------------------------------------------
/**
* The KVLSIHeuristicView class provides a representation of a result of a
* specific heuristic.
* @author Pascal Francq
*/
class KHGAHeuristicView : public KDevHGAView
{
	Q_OBJECT

	/**
	* Type of the heuristic.
	*/
	R::RString type;

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
	* Instance (only 1 chromosome).
	*/
	GInstH Inst;

	/**
	* Objects to group.
	*/
	RCursor<RObjH> Objs;

public:

	/**
	* Construct a heuristic view.
	*/
	KHGAHeuristicView(KDevHGADoc* pDoc,const R::RString& pType,QWidget* parent, const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual HGAViewType getType(void) {return(vHeuristic);}

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
	bool Running(void) {return(!Inst.GetHeuristic(0)->IsEnd());}

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
	~KHGAHeuristicView(void);
};


//-----------------------------------------------------------------------------
#endif
