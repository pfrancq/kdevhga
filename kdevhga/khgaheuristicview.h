/*

  khgaheuristicview.h

  Description - Header.

  (c) 2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

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
#include <rmath/random.h>
using namespace RMath;
#include <rhga/rhga.h>
#include <rhga/rtreeheuristic.h>
using namespace RHGA;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qlabel.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdevhgaview.h"
#include "qlistwords.h"
#include "gawords.h"


//-----------------------------------------------------------------------------
/**
* The KVLSIHeuristicView class provides a representation of a result of a
* specific heuristic.
* @author Pascal Francq
*/
class KHGAHeuristicView : public KDevHGAView, public RNodesGA<RNodeWords,RObjH,RNodeWordsData>
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
	QListWords* draw;

	/**
	* Heuristic used.
	*/
	RTreeHeuristic<RNodeWords,RObjH,RNodeWordsData>* TreeHeur;

	/**
	* Current information to treat.
	*/
	RObjH* CurObj;

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
	RNodeWordsData* Data;

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
