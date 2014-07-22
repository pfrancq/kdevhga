/*

	KHeuristic.h

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
#ifndef KHeuristic_H
#define KHeuristic_H


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//-----------------------------------------------------------------------------
// include files for current application
#include <ui_kheuristic.h>
#include <ginsth.h>
#include <kprjview.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
/**
* The KVLSIHeuristicView class provides a representation of a result of a
* specific heuristic.
* @author Pascal Francq
*/
class KHeuristic : public QMdiSubWindow, public Ui_KHeuristic
{
	Q_OBJECT

	/**
	 * Project
	*/
	KPrjView* Project;

	/**
	* Number of objects.
	*/
	unsigned int nbObjs;

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

public:

	/**
	* Construct a heuristic view.
	*/
	KHeuristic(KPrjView* project);

	/**
	* Start the heuristic.
	*/
	void RunHeuristic(void);

	/**
	* Next step for the choosen Heuristic.
	*/
	void NextStep(bool paint=true);

	/**
	* Run the heuristic to the end.
	*/
	void RunToEnd(void);

	/**
	* See if the heurisitic is running or not.
	* @return true if the heuristic is running.
	*/
	bool Running(void) {return(!Inst.GetHeuristic(0)->IsEnd());}

signals:

	/**
	* This signal indicates that the heuristic has finish to run.
	*/
	void endRun(void);

public:

	/**
	*  Destruct a heuristic view.
	*/
	~KHeuristic(void);
};


//-----------------------------------------------------------------------------
#endif
